import physis
from datetime import timedelta
import os


print("Running benchmark engine...")
t_total = 1.1
dt = 0.01
scalar = 1
tc = physis.TimeConfig(t_total, dt, scalar)
outdir = os.path.join(os.path.dirname(__file__), '')
render_time_ms = 10
render_time = timedelta(microseconds = render_time_ms * 1000)

engine = physis.BenchmarkEngine(tc, outdir, render_time)
engine.run()