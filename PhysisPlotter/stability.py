import physis
from datetime import datetime, timedelta
import os, sys, re, sqlite3
import numpy as np
import matplotlib.pyplot as plt
from dataclasses import dataclass

def build_timestamp_str() -> str:
    """Returns the current date-time with format YYYYMMDDHHMMSSMMM."""
    timestamp = datetime.now()
    return timestamp.strftime('%Y%m%d%H%M%S') + f"{timestamp.microsecond // 1000:03d}"


@dataclass(frozen=True)  # frozen because dict keys must be immutable
class SimulationMetadata:
    scalar: float
    dt: timedelta
    t_render: timedelta
    t_total: timedelta
    pcount: int


class EngineWrapper:
    def __init__(self, t_total=float, dt=float, scalar=float, render_time=timedelta, outdir=str):
        self.time_config = physis.TimeConfig(t_total, dt, scalar)
        self.outdir = outdir
        # TODO: Delay creation of engine
        self.engine = physis.BenchmarkEngine(self.time_config, self.outdir, render_time)

    def initialize(self, pcount=int) -> None:
        for _ in range(pcount):
            self.engine.add(physis.KinematicParameters())

    def initialize_one(self, rx=float, ry=float, vx=float, vy=float, ax=float, ay=float) -> None:
        r0 = physis.Vec2(rx, ry)
        v0 = physis.Vec2(vx, vy)
        a0 = physis.Vec2(ax, ay)
        self.engine.add(physis.KinematicParameters(r0, v0, a0))

    def run(self) -> None:
        self.engine.run()


class DataAggregator:
    def __init__(self, outdir=str):
        stability_results_filename = "stability.txt"
        trajectory_results_filename = "trajectory.txt"
        self.stability_expected_filepath = os.path.join(outdir, stability_results_filename)
        self.trajectory_expected_filepath = os.path.join(outdir, trajectory_results_filename)
        self.times_map = {}
        self.trajectory = {}

    def read_stability(self, metadata=SimulationMetadata) -> None:
        if not os.path.exists(self.stability_expected_filepath):
            raise FileNotFoundError(f"File not found: {self.stability_expected_filepath}")
        
        render_times = []
        with open(self.stability_expected_filepath, 'r') as file:
            for line in file:
                render_times.append(float(line))
        
        self.times_map[metadata] = np.array(render_times)

    def read_trajectory(self) -> None:
        if not os.path.exists(self.trajectory_expected_filepath):
            raise FileNotFoundError(f"File not found: {self.trajectory_expected_filepath}")
        
        self.trajectory = {}
        with open(self.trajectory_expected_filepath, 'r') as file:
            for i, line in enumerate(file):
                data = line.split('\t')
                pid = int(data[0])
                time = float(data[1])
                r_text = re.sub(r'[( )]', '', data[2]).split(',')
                r = (float(r_text[0]), float(r_text[1]))
                v_text = re.sub(r'[( )]', '', data[3]).split(',')
                v = (float(v_text[0]), float(v_text[1]))
                a_text = re.sub(r'[( )]', '', data[4]).split(',')
                a = (float(a_text[0]), float(a_text[1]))
                self.trajectory[i] = (pid, time, r, v, a)

    def serialize(self, filename=str) -> None:
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
        
        for m, data in self.times_map.items():
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
        
        for _, params in self.trajectory.items():
            pid = params[0]
            time = params[1]
            r = params[2]
            v = params[3]
            a = params[4]
            cursor.execute("INSERT INTO trajectory VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                           (pid, time, r[0], r[1], v[0], v[1], a[0], a[1]))
            connection.commit()
        
        connection.close()
        self.times_map = {}

    def deserialize(self, filename=str) -> None:
        self.times_map = {}
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
                                          timedelta(milliseconds=dt),
                                          timedelta(milliseconds=render_time), 
                                          timedelta(milliseconds=t_total),
                                          pcount)
            # hack
            data = np.zeros(row[7])
            self.times_map[metadata] = np.frombuffer(row[6], dtype=data.dtype)

        # Trajectory
        trajectory_rows = cursor.execute("SELECT pid, time, rx, ry, vx, vy, ax, ay FROM trajectory").fetchall()
        for i, row in enumerate(trajectory_rows):
            pid = row[0]
            time = row[1]
            r = (row[2], row[3])
            v = (row[4], row[5])
            a = (row[6], row[7])
            self.trajectory[i] = (pid, time, r, v, a)
        connection.close()


class Plotter:
    def __init__(self, aggregator=DataAggregator):
        self.aggregator = aggregator

    def _get_metadata_label(self, m=SimulationMetadata) -> str:
        return (f"r={m.scalar:.2f},"
                f"dt={m.dt.microseconds / 1000:.2f},"
                f"N={m.pcount},"
                f"tr={m.t_render.microseconds / 1000:.2f},"
                f"tt={m.t_total.microseconds / 1000:.2f}")

    def plot_stability(self):
        # Clip the initial frame because it is fast
        fig, ax = plt.subplots()
        for metadata, times in self.aggregator.times_map.items():
            x = np.arange(len(times) - 1)
            y = times[1:]
            ax.scatter(x, y, vmin=0, vmax=100, label=self._get_metadata_label(metadata))
            ax.legend()
        plt.show()

    def plot_trajectory(self):
        x, y = [], []
        fig, ax = plt.subplots()
        for i, params in self.aggregator.trajectory.items():
            pid, time, r, v, a = params[0], params[1], params[2], params[3], params[4]
            x.append(r[0])
            y.append(r[1])
        ax.scatter(x, y)
        plt.show()


if __name__ == "__main__":
    args = sys.argv
    outdir = os.path.join(os.path.dirname(__file__), '')
    aggregator = DataAggregator(outdir)
    db_name = "results.db"

    if len(args) > 1 and args[1] == "run":
        print("Running benchmark engine...")
        t_total, scalar, render_time_ms = 2, 1, 1

        for dt in [0.01]:
            for pcount in [1]:
                print(f"Executing run: dt={dt}, pcount={pcount}, t_total={t_total}, render_time={render_time_ms}, scalar={scalar}")
                render_time = timedelta(milliseconds=render_time_ms)
                engine = EngineWrapper(t_total, dt, scalar, render_time, outdir)
                #engine.initialize(pcount)
                engine.initialize_one(0, 0, 10, 10, 0, -9.81)
                engine.run()
                print("Run complete.  Getting ms per frame...")

                metadata = SimulationMetadata(scalar, 
                                              timedelta(milliseconds=dt),
                                              render_time, 
                                              timedelta(milliseconds=t_total),
                                              pcount)
                aggregator.read_stability(metadata)
                aggregator.read_trajectory()
                print("Data saved.")
        
        aggregator.serialize(db_name)
    
    aggregator.deserialize(db_name)
    plotter = Plotter(aggregator)
    plotter.plot_stability()
    plotter.plot_trajectory()
    