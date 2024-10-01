import physis
from datetime import datetime, timedelta
import os, sys, re, sqlite3
import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass
from enum import Enum
from typing import Dict
from functools import reduce


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

trajectory_params = KinematicData(0, timedelta(seconds=0), 0, 0, 10, 10, 0, -9.81)


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

    def initialize(self, rx: float, ry: float, vx: float, vy: float, ax: float, ay: float) -> None:
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


class StabilityReader:
    def __init__(self, path: str):
        self.path = path
        self.times = {}  # metadata -> times

    def cache(self, metadata: SimulationMetadata) -> None:
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"File not found: {self.path}")
        
        self.times, render_times = {}, []
        with open(self.path, 'r') as file:
            for line in file:
                render_times.append(float(line))
        
        self.times[metadata] = np.array(render_times)

    def write(self, connection: sqlite3.Connection, timestamp_str: str, overwrite: bool, dump_data: bool) -> None:
        cursor = connection.cursor()
        if overwrite: cursor.execute("DROP TABLE IF EXISTS stability")

        cursor.execute(("CREATE TABLE IF NOT EXISTS stability (datetime TEXT, "
                "dt FLOAT, "
                "pcount INTEGER, "
                "scalar FLOAT,"
                "rendertime FLOAT," 
                "totaltime FLOAT," 
                "data BLOB," 
                "datalen INT)"))
        
        for m, data in self.times.items():
            datablob = data.tobytes()
            dt = m.dt.microseconds / 1000
            t_render = m.t_render.microseconds / 1000
            t_total = m.t_total.microseconds / 1000
            pcount = m.pcount
            scalar = m.scalar
            cursor.execute("INSERT INTO stability VALUES (?, ?, ?, ?, ?, ?, ?, ?)", 
                (timestamp_str, dt, pcount, scalar, t_render, t_total, datablob, len(data)))
            connection.commit()
        
        if dump_data: self.times = {}

    def read(self, connection: sqlite3.Connection) -> None:
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
            self.times[metadata] = np.frombuffer(row[6], dtype=data.dtype)


class TrajectoryReader:
    def __init__(self, path: str):
        self.path = path
        self.trajectory = {}  # time_id -> kinematic param

    def cache(self, metadata: SimulationMetadata) -> None:  # TODO: Get rid of metadata, only for stability
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"File not found: {self.path}")
        
        self.trajectory = {}
        with open(self.path, 'r') as file:
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
                self.trajectory[i] = KinematicData(pid, time, rx, ry, vx, vy, ax, ay)

    def write(self, connection: sqlite3.Connection, timestamp_str: str, overwrite: bool, dump_data: bool) -> None:
        particleids = set([data.pid for data in self.trajectory.values()])
        pcount = len(particleids)

        print(f"TEMP.  pcount={pcount}")
        totaltime_sec = reduce(lambda t1, t2: t1 + t2, [data.time.total_seconds() for data in self.trajectory.values()])
        print(f"TEMP.  total time (sec)={totaltime_sec}")

        cursor = connection.cursor()
        if overwrite:
            cursor.execute("DROP TABLE IF EXISTS Trajectories")
            cursor.execute("DROP TABLE IF EXISTS Particles")
            cursor.execute("DROP TABLE IF EXISTS ParticleStates")

        cursor.execute(
            ("CREATE TABLE IF NOT EXISTS Trajectories ("
                "RUNID	    INTEGER,"
                "TIMESTAMP	TEXT,"
                "PCOUNT	    INTEGER,"
                "RUNTIME	FLOAT,"
                "PRIMARY KEY(RUNID AUTOINCREMENT)"
            ")"))
        
        cursor.execute("INSERT INTO Trajectories (TIMESTAMP, PCOUNT, RUNTIME) VALUES (?, ?, ?)",
                           (timestamp_str, pcount, totaltime_sec))
        runid = cursor.execute("SELECT MAX(RUNID) FROM Trajectories").fetchall()[0][0]
        
        cursor.execute(
            ("CREATE TABLE IF NOT EXISTS Particles ("
                "PID	    INTEGER,"
                "RUNID	    INTEGER,"
                "PRIMARY KEY(PID AUTOINCREMENT),"
                "FOREIGN KEY(RUNID) REFERENCES Trajectories(RUNID) ON DELETE CASCADE"
            ")"))
        print(particleids)
        for pid in particleids:
            cursor.execute("INSERT INTO Particles (PID, RUNID) VALUES (?, ?)", (pid, runid))
        
        cursor.execute(
            ("CREATE TABLE IF NOT EXISTS ParticleStates ("
                "STATEID	INTEGER,"
                "PID	    INTEGER,"
                "SIMTIME	FLOAT,"
                "RX	        FLOAT,"
                "RY	        FLOAT,"
                "VX	        FLOAT,"
                "VY	        FLOAT,"
                "AX	        FLOAT,"
                "AY	        FLOAT,"
                "PRIMARY KEY(STATEID AUTOINCREMENT),"
                "FOREIGN KEY(PID) REFERENCES Particles(PID) ON DELETE CASCADE"
            ")"))
        
        print(f"TEMP.  runid={runid}")
        for _, data in self.trajectory.items():
            cursor.execute("INSERT INTO ParticleStates (PID, SIMTIME, RX, RY, VX, VY, AX, AY) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                           (data.pid, data.time.total_seconds(), data.rx, data.ry, data.vx, data.vy, data.ax, data.ay))
            connection.commit()

        if dump_data: self.trajectory = {}

    def read(self, connection: sqlite3.Connection) -> None:
        cursor = connection.cursor()
        runid = int(cursor.execute("SELECT MAX(RUNID) FROM Trajectories").fetchall()[0][0])
        particle_states = cursor.execute("SELECT * FROM ParticleStates ps JOIN Particles p ON ps.pid = p.pid JOIN Trajectories t ON p.runid = t.runid WHERE t.runid = (?)", (runid,)).fetchall()
        for i, row in enumerate(particle_states):
            pid = row[1]
            time = row[2]
            r = (row[3], row[4])
            v = (row[5], row[6])
            a = (row[7], row[8])
            self.trajectory[i] = (pid, time, r, v, a)


class DataAggregator:
    def __init__(self, filepaths: Dict[DataType, str]):
        if filepaths is None:
            raise ValueError("Supplied datafiles must be a non-empty dict!")
        
        self.readers = {}
        for type, path in filepaths.items():
            self.readers[type] = self._create_reader(type, path)

    def _create_reader(self, type: DataType, path: str):  # TODO: Create interface
        if type == DataType.STABILITY:  return StabilityReader(path)
        if type == DataType.TRAJECTORY: return TrajectoryReader(path)

    def cache(self, metadata: SimulationMetadata) -> None:
        for reader in self.readers.values():
            reader.cache(metadata)  # TODO: Only needed for stability reader
    # TODO: replace with store_all, add store(DataType)

    def write(self, dbfilename: str, overwrite: bool, dump_data: bool) -> None:  # todo: replace with write_all, add write(DataType)
        timestamp_str = build_timestamp_str()
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.write(connection, timestamp_str, overwrite, dump_data)
        
        connection.close()

    def read(self, dbfilename: str) -> None:
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.read(connection)
        
        connection.close()


class Plotter:
    def __init__(self, aggregator: DataAggregator):  # TODO: Replace with dict of readers
        if not aggregator.readers[DataType.STABILITY].times or not aggregator.readers[DataType.TRAJECTORY].trajectory:
            raise RuntimeError("Aggregator must contain both stability and trajectory data!")
        self.aggregator = aggregator

    def _get_metadata_label(self, m: SimulationMetadata) -> str:
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
        for metadata, times in self.aggregator.readers[DataType.STABILITY].times.items():
            x = np.arange(len(times) - 1)
            y = times[1:]
            ax.scatter(x, y, vmin=0, vmax=100, label=self._get_metadata_label(metadata))
            ax.legend()
        plt.show()

    def plot_trajectory(self):
        x, y = [], []
        fig, ax = plt.subplots(1)

        # Trajectory plot
        ax.set_title("Particle Trajectory"), ax.set_xlabel("x (m)"), ax.set_ylabel("y (m)")
        ax.grid(True, zorder=1)
        
        for i, params in self.aggregator.readers[DataType.TRAJECTORY].trajectory.items():
            pid, time, r, v, a = params[0], params[1], params[2], params[3], params[4]
            x.append(r[0])
            y.append(r[1])
        
        polynomial = self._generate_polynomial(trajectory_params)  # TODO: Inject as method parameter
        print(polynomial.coefficients)
        quadvals = np.arange(min(x), max(x), 0.01)
        ax.plot(quadvals, polynomial(quadvals), label=f"Analytic solution: y(x) = {polynomial.coefficients[0]:.3f}$t^2$ + {polynomial.coefficients[1]:.3f}t + {polynomial.coefficients[2]:.3f}", color='Grey')
        ax.scatter(x, y, s=10, label='Raw data', color='Red', alpha=0.75, zorder=2)
        plt.legend()

        # Error plot
        #ax[1].set_title("Location errors"), ax[1].set_xlabel()
        plt.show()

    def _generate_polynomial(self, params: KinematicData) -> np.poly1d:
        if params.ax != 0:
            raise NotImplementedError("No logic implemented for nonzero x accelerations!")
        if params.vx == 0:
            raise NotImplementedError("Initial vx must be nonzero!")
        
        x0, y0, vx0, vy0, ay = params.rx, params.ry, params.vx, params.vy, params.ay
        c1 = ay / (vx0 ** 2)
        c2 = vy0 / vx0
        return np.poly1d([0.5 * c1, c2 - (c1 * x0), y0 - (c2 * x0) + (0.5 * c1 * (x0**2))])


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
                engine.initialize(trajectory_params.rx, trajectory_params.ry, trajectory_params.vx, trajectory_params.vy, trajectory_params.ax, trajectory_params.ay)
                engine.run()
                print("Run complete.  Collecting data...")
                aggregator.cache(metadata)  # TODO: map datatype enum -> reqd object
                print("Data stored.")
        
        aggregator.write(db_name, True, False)

    aggregator.read(db_name)
    plotter = Plotter(aggregator)
    plotter.plot(DataType.STABILITY)
    plotter.plot(DataType.TRAJECTORY)
