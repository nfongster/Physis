import physis
from datetime import datetime, timedelta
import os, sys, re, sqlite3
import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass
from enum import Enum
from typing import Dict


db_name = "results.db"
stability_filename = "stability.txt"
trajectory_filename = "trajectory.txt"


def build_timestamp_str() -> str:
    """Returns the current date-time with format YYYYMMDDHHMMSSMMM."""
    timestamp = datetime.now()
    return timestamp.strftime('%Y%m%d%H%M%S') + f"{timestamp.microsecond // 1000:03d}"


@dataclass(frozen=True)  # frozen because dict keys must be immutable
class KinematicData:
    pid:  int        # Particle ID.
    time: timedelta  # Time relative to start of simulation.
    rx:   float      # X position.
    ry:   float      # Y position.
    vx:   float      # X velocity.
    vy:   float      # Y velocity.
    ax:   float      # X acceleration.
    ay:   float      # Y acceleration.


@dataclass(frozen=True)
class SimulationMetadata:
    scalar:   float      # Determines simulation speed.
    dt:       timedelta  # Simulation integration time step size.
    t_render: timedelta  # Time to simulate rendering.
    t_total:  timedelta  # Total time to run the simulation.
    pcount:   int        # Number of particles in the simulation.


class DataType(Enum):
    STABILITY = 0
    TRAJECTORY = 1


class EngineWrapper:
    def __init__(self, metadata=SimulationMetadata, outdir=str):
        self.metadata = metadata
        self.outdir = outdir
        self.engine = None

    def initialize(self) -> None:
        self._create_engine()
        for _ in range(self.metadata.pcount):
            self.engine.add(physis.KinematicParameters())

    def initialize(self, rx=float, ry=float, vx=float, vy=float, ax=float, ay=float) -> None:
        r0 = physis.Vec2(rx, ry)
        v0 = physis.Vec2(vx, vy)
        a0 = physis.Vec2(ax, ay)
        self._create_engine()
        for _ in range(self.metadata.pcount):
            self.engine.add(physis.KinematicParameters(r0, v0, a0))

    def run(self) -> None:
        if self.engine == None:
            self.initialize()
        self.engine.run()

    def _create_engine(self) -> None:
        config = physis.TimeConfig(self.metadata.t_total, self.metadata.dt, self.metadata.scalar)
        self.engine = physis.BenchmarkEngine(config, self.outdir, self.metadata.t_render)


class DataAggregator:
    def __init__(self, filepaths: Dict[DataType, str]):
        if filepaths is None:
            raise ValueError("Supplied datafiles must be a non-empty dict!")
        
        self.filepaths = filepaths
        self.map_metadata_to_times = {}
        self.map_timeid_to_trajectory = {}

    def store(self, metadata=SimulationMetadata) -> None:
        self.read_stability(metadata)
        self.read_trajectory()

    def read_stability(self, metadata=SimulationMetadata) -> None:
        stability_filepath = self.filepaths[DataType.STABILITY]
        if not os.path.exists(stability_filepath):
            raise FileNotFoundError(f"File not found: {stability_filepath}")
        
        render_times = []
        with open(stability_filepath, 'r') as file:
            for line in file:
                render_times.append(float(line))
        
        self.map_metadata_to_times[metadata] = np.array(render_times)

    def read_trajectory(self) -> None:
        trajectory_filepath = self.filepaths[DataType.TRAJECTORY]
        if not os.path.exists(trajectory_filepath):
            raise FileNotFoundError(f"File not found: {trajectory_filepath}")
        
        self.map_timeid_to_trajectory = {}
        with open(trajectory_filepath, 'r') as file:
            for i, line in enumerate(file):
                data = line.split('\t')
                pid = int(data[0])
                time = timedelta(seconds=float(data[1]))
                r_text = re.sub(r'[( )]', '', data[2]).split(',')
                rx, ry = float(r_text[0]), float(r_text[1])
                v_text = re.sub(r'[( )]', '', data[3]).split(',')
                vx, vy = float(v_text[0]), float(v_text[1])
                a_text = re.sub(r'[( )]', '', data[4]).split(',')
                ax, ay = float(a_text[0]), float(a_text[1])
                self.map_timeid_to_trajectory[i] = KinematicData(pid, time, rx, ry, vx, vy, ax, ay)

    def write(self, filename=str, dump_data=bool) -> None:
        timestamp_str = build_timestamp_str()
        connection = sqlite3.connect(filename)
        cursor = connection.cursor()
        # Stability
        cursor.execute(("CREATE TABLE stability (datetime TEXT, "
                "dt FLOAT, "
                "pcount INTEGER, "
                "scalar FLOAT,"
                "rendertime FLOAT," 
                "totaltime FLOAT," 
                "data BLOB," 
                "datalen INT)"))
        
        for m, data in self.map_metadata_to_times.items():
            datablob = data.tobytes()
            dt = m.dt.microseconds / 1000
            t_render = m.t_render.microseconds / 1000
            t_total = m.t_total.microseconds / 1000
            pcount = m.pcount
            scalar = m.scalar
            cursor.execute("INSERT INTO stability VALUES (?, ?, ?, ?, ?, ?, ?, ?)", 
                (timestamp_str, dt, pcount, scalar, t_render, t_total, datablob, len(data)))
        
            connection.commit()

        # Trajectory
        cursor.execute(("CREATE TABLE trajectory (pid INTEGER,"
                "time FLOAT,"
                "rx FLOAT,"
                "ry FLOAT,"
                "vx FLOAT,"
                "vy FLOAT,"
                "ax FLOAT,"
                "ay FLOAT)"))
        
        for _, data in self.map_timeid_to_trajectory.items():
            cursor.execute("INSERT INTO trajectory VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                           (data.pid, data.time.total_seconds(), data.rx, data.ry, data.vx, data.vy, data.ax, data.ay))
            connection.commit()
        
        connection.close()
        if dump_data:
            self.map_metadata_to_times = {}
            self.map_timeid_to_trajectory = {}

    def read(self, filename=str) -> None:
        self.map_metadata_to_times = {}
        connection = sqlite3.connect(filename)
        cursor = connection.cursor()
        rows = cursor.execute("SELECT datetime, dt, pcount, scalar, rendertime, totaltime, data, datalen FROM stability").fetchall()
        for row in rows:
            dt = row[1]
            pcount = row[2]
            scalar = row[3]
            render_time = row[4]
            t_total = row[5]
            metadata = SimulationMetadata(scalar, 
                                          timedelta(seconds=dt),
                                          timedelta(seconds=render_time), 
                                          timedelta(seconds=t_total),
                                          pcount)
            # hack
            data = np.zeros(row[7])
            self.map_metadata_to_times[metadata] = np.frombuffer(row[6], dtype=data.dtype)

        # Trajectory
        trajectory_rows = cursor.execute("SELECT pid, time, rx, ry, vx, vy, ax, ay FROM trajectory").fetchall()
        for i, row in enumerate(trajectory_rows):
            pid = row[0]
            time = row[1]
            r = (row[2], row[3])
            v = (row[4], row[5])
            a = (row[6], row[7])
            self.map_timeid_to_trajectory[i] = (pid, time, r, v, a)
        connection.close()


class Plotter:
    def __init__(self, aggregator=DataAggregator):
        if not aggregator.map_metadata_to_times or not aggregator.map_timeid_to_trajectory:
            raise RuntimeError("Aggregator must contain both stability and trajectory data!")
        self.aggregator = aggregator

    def _get_metadata_label(self, m=SimulationMetadata) -> str:
        return (f"r={m.scalar:.2f},"
                f"dt={m.dt.microseconds / 1000:.2f},"
                f"N={m.pcount},"
                f"tr={m.t_render.microseconds / 1000:.2f},"
                f"tt={m.t_total.microseconds / 1000:.2f}")
    
    def plot(self, type: DataType):
        if type == DataType.STABILITY:
            self.plot_stability()
        elif type == DataType.TRAJECTORY:
            self.plot_trajectory()

    def plot_stability(self):
        # Clip the initial frame because it is fast
        fig, ax = plt.subplots()
        for metadata, times in self.aggregator.map_metadata_to_times.items():
            x = np.arange(len(times) - 1)
            y = times[1:]
            ax.scatter(x, y, vmin=0, vmax=100, label=self._get_metadata_label(metadata))
            ax.legend()
        plt.show()

    def plot_trajectory(self):
        x, y = [], []
        fig, ax = plt.subplots()
        for i, params in self.aggregator.map_timeid_to_trajectory.items():
            pid, time, r, v, a = params[0], params[1], params[2], params[3], params[4]
            x.append(r[0])
            y.append(r[1])
        ax.scatter(x, y)
        plt.show()


if __name__ == "__main__":
    args = sys.argv
    outdir = os.path.join(os.path.dirname(__file__), '')
    filepaths = { 
        DataType.STABILITY : os.path.join(stability_filename), 
        DataType.TRAJECTORY : os.path.join(trajectory_filename) }
    aggregator = DataAggregator(filepaths)

    if len(args) > 1 and args[1] == "run":
        print("Running benchmark engine...")
        t_total, scalar, render_time = timedelta(seconds=2), 1, timedelta(seconds=0.001)

        for dt in [timedelta(seconds=0.01)]:
            for pcount in [1]:
                print(f"Executing run: dt={dt}, pcount={pcount}, t_total={t_total}, render_time={render_time}, scalar={scalar}")
                metadata = SimulationMetadata(scalar, dt, render_time, t_total, pcount)
                engine = EngineWrapper(metadata, outdir)
                engine.initialize(0, 0, 10, 10, 0, -9.81)
                engine.run()
                print("Run complete.  Collecting data...")
                aggregator.store(metadata)  # TODO: map datatype enum -> reqd object
                print("Data stored.")
        
        aggregator.write(db_name, False)

    aggregator.read(db_name)
    plotter = Plotter(aggregator)
    plotter.plot(DataType.STABILITY)
    plotter.plot(DataType.TRAJECTORY)
