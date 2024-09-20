import physis
from datetime import timedelta
import os
import numpy as np
import matplotlib.pyplot as plt


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
        self.times_array = []

    def read(self) -> np.array:
        if not os.path.exists(self.expected_filepath):
            raise FileNotFoundError(f"File not found: {self.expected_filepath}")
        
        render_times = []
        with open(self.expected_filepath, 'r') as file:
            for line in file:
                render_times.append(float(line))
        return np.array(render_times)
    
    def save(self, times=np.array) -> None:
        self.times_array.append(times)


class Plotter:
    def __init__(self, aggregator=DataAggregator):
        self.aggregator = aggregator
        self.fig, self.ax = plt.subplots()

    def plot(self):
        # Clip the initial frame because it is fast
        for times in self.aggregator.times_array:
            x = np.arange(len(times) - 1)
            y = times[1:]
            self.ax.scatter(x, y, vmin=0, vmax=100)
        plt.show()


if __name__ == "__main__":
    print("Running benchmark engine...")
    t_total = 10
    dt = 0.01
    scalar = 1
    render_time_ms = 1
    pcount = 1
    outdir = os.path.join(os.path.dirname(__file__), '')
    aggregator = DataAggregator(outdir)

    for dt in [0.01, 0.02, 0.03]:
        print(f"Executing run dt={dt}...")
        render_time = timedelta(microseconds = render_time_ms * 1000)
        engine = EngineWrapper(t_total, dt, scalar, render_time, outdir)
        engine.initialize(pcount)
        engine.run()
        print(f"Run dt={dt} complete.  Getting ms per frame...")

        times = aggregator.read()
        aggregator.save(times)
        print("Data saved.")

    plotter = Plotter(aggregator)
    plotter.plot()
    