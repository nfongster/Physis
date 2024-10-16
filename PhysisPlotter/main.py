import physis
from datetime import timedelta
import os, sys
import numpy as np
import matplotlib.pyplot as plt
import matplotlib.cm as cm
from aggregator import *
from utility import *


db_name = "results.db"
trajectory_params = KinematicData(0, timedelta(seconds=0), 0, 0, 10, 70, 0, -9.81)
filepaths = { 
    DataType.STABILITY : os.path.join("stability.txt"), 
    DataType.TRAJECTORY : os.path.join("trajectory.txt") 
}


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


class Plotter:
    def __init__(self, aggregator: DataAggregator):  # TODO: Replace with dict of readers
        if not aggregator.readers[DataType.STABILITY].times or not aggregator.readers[DataType.TRAJECTORY].trajectory:
            raise RuntimeError("Aggregator must contain both stability and trajectory data!")
        self.aggregator = aggregator

    def _get_metadata_label(self, m: SimulationMetadata) -> str:
        return (f"r={m.scalar:.2f},"
                f"dt={m.dt.total_seconds():.3f},"
                f"N={m.pcount},"
                f"tr={m.t_render.total_seconds():.3f},"
                f"tt={m.t_total.total_seconds():.3f}")
    
    def plot(self, type: DataType, initconditions: KinematicData):
        if type == DataType.STABILITY:
            self.plot_stability()
        elif type == DataType.TRAJECTORY:
            self.plot_trajectory(initconditions)

    def plot_stability(self):
        # Clip the initial frame because it is fast
        fig, ax = plt.subplots()
        ax.set_xlabel("Frame"), ax.set_ylabel("Time (ms)")
        for timestamp, stabilitydata in self.aggregator.readers[DataType.STABILITY].times.items():
            metadata, times = stabilitydata.metadata, stabilitydata.times
            x = np.arange(len(times) - 1)
            y = times[1:]
            ax.scatter(x, y, vmin=0, vmax=100, label=self._get_metadata_label(metadata))
            ax.legend()
        plt.show()

    def plot_trajectory(self, initconditions: KinematicData):
        fig, ax = plt.subplots(2)
        polynomial = self._generate_polynomial(initconditions)

        # Trajectory plot
        ax[0].set_title("Particle Trajectory"), ax[0].set_xlabel("x (m)"), ax[0].set_ylabel("y (m)")
        ax[0].grid(True, zorder=1)

        # Error plot
        ax[1].set_title("Location errors"), ax[1].set_xlabel("x (m)"), ax[1].set_ylabel("Relative Error, %: $(y_{actual} - y_{analytic}) / y_{analytic}$")
        ax[1].grid(True, zorder=1)
        
        first_iter = True
        colormap = cm.rainbow(np.linspace(0, 1, len(self.aggregator.readers[DataType.TRAJECTORY].trajectory.values())))
        for i, trajdata in enumerate(self.aggregator.readers[DataType.TRAJECTORY].trajectory.values()):
            x, y = [], []
            metadata, trajectories = trajdata.metadata, trajdata.trajectories
            for params in trajectories.values():  # TODO: account for multiple particles
                x.append(params.rx)
                y.append(params.ry)

            if first_iter:
                quadvals = np.arange(min(x), max(x), 0.01)
                ax[0].plot(quadvals, polynomial(quadvals), label=f"Analytic solution: y(x) = {polynomial.coefficients[0]:.3f}$t^2$ + {polynomial.coefficients[1]:.3f}t + {polynomial.coefficients[2]:.3f}", color='Grey')
                first_iter = False
            
            ax[0].scatter(x, y, s=10, label=self._get_metadata_label(metadata), alpha=0.75, color=colormap[i], zorder=2)
            ax[1].plot(x, 100 * (y - polynomial(x)) / polynomial(x), label=self._get_metadata_label(metadata), color=colormap[i], zorder=2)
        ax[0].legend(), ax[1].legend()
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
    aggregator = DataAggregator(filepaths)

    if len(args) > 1 and args[1] == "run":
        print("Running benchmark engine...")
        t_total, scalar, render_time = timedelta(seconds=12), 1, timedelta(seconds=0.001)

        first_iter = True
        for dt_sec in [0.0001, 0.001, 0.01]:
            for pcount in [1]:
                dt = timedelta(seconds=dt_sec)
                print(f"Executing run: dt={dt}, pcount={pcount}, t_total={t_total}, render_time={render_time}, scalar={scalar}")
                metadata = SimulationMetadata(scalar, dt, render_time, t_total, pcount)
                engine = EngineWrapper(metadata, outdir)
                engine.initialize(trajectory_params.rx, trajectory_params.ry, trajectory_params.vx, trajectory_params.vy, trajectory_params.ax, trajectory_params.ay)
                engine.run()
                print("Run complete.  Collecting data...")
                aggregator.cache(metadata)
                print("Data stored.")
                if first_iter and len(args) > 2 and args[2] == "reset":
                    aggregator.delete(db_name)
                    first_iter = False
                aggregator.write(db_name, True)  # You must reset the cache, otherwise you'll get duplicate entries in the database
                print("Data saved to database.")
    
    aggregator.read(db_name)
    plotter = Plotter(aggregator)
    plotter.plot(DataType.STABILITY, trajectory_params)
    plotter.plot(DataType.TRAJECTORY, trajectory_params)
