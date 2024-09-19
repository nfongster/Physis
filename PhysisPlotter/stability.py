import physis
from datetime import timedelta
import os
import numpy as np
import matplotlib.pyplot as plt

results_filename = "results.txt"


if __name__ == "__main__":
    print("Running benchmark engine...")
    # Time parameters
    t_total = 10
    dt = 0.01
    scalar = 1
    tc = physis.TimeConfig(t_total, dt, scalar)

    # Output directory
    outdir = os.path.join(os.path.dirname(__file__), '')

    # Rendering time per frame
    render_time_ms = 10
    render_time = timedelta(microseconds = render_time_ms * 1000)

    # Build engine
    engine = physis.BenchmarkEngine(tc, outdir, render_time)

    # Add particles
    pcount = 100
    for _ in range(pcount):
        engine.add()

    # Run engine
    engine.run()
    print("Run complete.  Getting ms per frame...")

    # Check if txt file exists
    expected_filepath = os.path.join(outdir, results_filename)
    if not os.path.exists(expected_filepath):
        raise FileNotFoundError(f"File not found: {expected_filepath}")
    
    # Get contents of txt file
    render_times = []
    with open(expected_filepath, 'r') as file:
        for line in file:
            render_times.append(float(line))

    # Make and print array
    times = np.array(render_times)
    print("Printing ms per frame...\n")
    print(times)

    # Plot data (clip the initial frame because it is fast)
    fig, ax = plt.subplots()
    x = np.arange(len(times) - 1)
    y = times[1:]
    ax.scatter(x, y, vmin=0, vmax=100)
    plt.show()