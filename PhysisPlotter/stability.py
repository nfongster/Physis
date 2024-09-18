import physis
from datetime import timedelta
import os

results_filename = "results.txt"


def main():
    print("Running benchmark engine...")
    # Time parameters
    t_total = 1.1
    dt = 0.001
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
    for _ in range(10):
        engine.add()

    # Run engine
    engine.run()
    print("Run complete.  Printing ms per frame...\n")

    # Check if txt file exists
    expected_filepath = os.path.join(outdir, results_filename)
    if not os.path.exists(expected_filepath):
        raise FileNotFoundError(f"File not found: {expected_filepath}")
    
    # Print contents of txt file
    with open(expected_filepath, 'r') as file:
        for line in file:
            print(line, end='')


if __name__ == "__main__":
    main()