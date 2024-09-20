import physis
from datetime import timedelta
import os, sys
import numpy as np
import pickle
import matplotlib.pyplot as plt
from dataclasses import dataclass

@dataclass(frozen=True)  # frozen because dict keys must be immutable
class SimulationMetadata:
    scalar: timedelta
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
            self.engine.add()

    def run(self) -> None:
        self.engine.run()


class DataAggregator:
    def __init__(self, outdir=str):
        results_filename = "results.txt"
        self.expected_filepath = os.path.join(outdir, results_filename)
        self.times_map = {}

    def read(self, metadata=SimulationMetadata) -> None:
        if not os.path.exists(self.expected_filepath):
            raise FileNotFoundError(f"File not found: {self.expected_filepath}")
        
        render_times = []
        with open(self.expected_filepath, 'r') as file:
            for line in file:
                render_times.append(float(line))
        
        self.times_map[metadata] = np.array(render_times)

    def serialize(self, filename=str) -> None:
        with open(filename, 'wb') as f:
            pickle.dump(self.times_map, f)
        self.times_map = {}

    def deserialize(self, filename=str) -> None:
        self.times_map = {}
        with open(filename, 'rb') as f:
            self.times_map = pickle.load(f)


class Plotter:
    def __init__(self, aggregator=DataAggregator):
        self.aggregator = aggregator
        self.fig, self.ax = plt.subplots()

    def _get_metadata_label(self, m=SimulationMetadata) -> str:
        return (f"r={m.scalar.microseconds / 1000:.2f},"
                f"dt={m.dt.microseconds / 1000:.2f},"
                f"N={m.pcount},"
                f"tr={m.t_render.microseconds / 1000:.2f},"
                f"tt={m.t_total.microseconds / 1000:.2f}")

    def plot(self):
        # Clip the initial frame because it is fast
        for metadata, times in self.aggregator.times_map.items():
            x = np.arange(len(times) - 1)
            y = times[1:]
            self.ax.scatter(x, y, vmin=0, vmax=100, label=self._get_metadata_label(metadata))
            self.ax.legend()
        plt.show()


if __name__ == "__main__":
    args = sys.argv
    outdir = os.path.join(os.path.dirname(__file__), '')
    aggregator = DataAggregator(outdir)
    
    if len(args) > 1 and args[1] == "run":
        print("Running benchmark engine...")
        t_total = 10
        dt = 0.01
        scalar = 1
        render_time_ms = 1
        pcount = 1
    

        for dt in [0.01, 0.02, 0.03]:
            print(f"Executing run dt={dt}...")
            render_time = timedelta(milliseconds=render_time_ms)
            engine = EngineWrapper(t_total, dt, scalar, render_time, outdir)
            engine.initialize(pcount)
            engine.run()
            print(f"Run dt={dt} complete.  Getting ms per frame...")

            metadata = SimulationMetadata(timedelta(milliseconds=scalar), 
                                          timedelta(milliseconds=dt),
                                          render_time, 
                                          timedelta(milliseconds=t_total),
                                          pcount)
            aggregator.read(metadata)
            print("Data saved.")

        aggregator.serialize("aggregate_results.pkl")
    
    aggregator.deserialize("aggregate_results.pkl")
    plotter = Plotter(aggregator)
    plotter.plot()
    