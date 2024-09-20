import physis
from datetime import timedelta
import os
import numpy as np
import matplotlib.pyplot as plt


class EngineWrapper:
    def __init__(self, t_total=float, dt=float, scalar=float, render_time=timedelta):
        self.time_config = physis.TimeConfig(t_total, dt, scalar)
        self.outdir = os.path.join(os.path.dirname(__file__), '')
        # TODO: Delay creation of engine
        self.engine = physis.BenchmarkEngine(self.time_config, self.outdir, render_time)

    def initialize(self, pcount=int) -> None:
        for _ in range(pcount):
            self.engine.add()

    def run(self) -> None:
        self.engine.run()


class DataCollector:
    def __init__(self, outdir=str):
        results_filename = "results.txt"
        self.expected_filepath = os.path.join(outdir, results_filename)

    def read(self) -> np.array:
        if not os.path.exists(self.expected_filepath):
            raise FileNotFoundError(f"File not found: {self.expected_filepath}")
        
        render_times = []
        with open(self.expected_filepath, 'r') as file:
            for line in file:
                render_times.append(float(line))
        return np.array(render_times)


class Plotter:
    def __init__(self, times=np.array):
        self.times = times
        self.fig, self.ax = plt.subplots()

    def plot(self):
        # Clip the initial frame because it is fast
        x = np.arange(len(self.times) - 1)
        y = self.times[1:]
        self.ax.scatter(x, y, vmin=0, vmax=100)
        plt.show()


if __name__ == "__main__":
    print("Running benchmark engine...")
    t_total = 10
    dt = 0.01
    scalar = 1
    render_time_ms = 1
    render_time = timedelta(microseconds = render_time_ms * 1000)
    pcount = 1

    engine = EngineWrapper(t_total, dt, scalar, render_time)
    engine.initialize(pcount)
    engine.run()
    print("Run complete.  Getting ms per frame...")

    collector = DataCollector(engine.outdir)
    times = collector.read()
    print("Printing ms per frame...\n")
    print(times)

    plotter = Plotter(times)
    plotter.plot()
    