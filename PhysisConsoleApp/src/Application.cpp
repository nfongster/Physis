#include <iostream>
#include <Physis.h>
#include "Utility.h"
#include "Extractor.h"
#include "ConsoleExtractor.h"
#include "FileExtractor.h"

// Make a simple GUI that does the following:
// - Shows trajectory of particle in 2D under gravity
// - Allows user to specify initial position and velocity
// - Allows user to specify distance scale (e.g., window = 1 km across)
// - Shows current velocity, position, acceleration, and elapsed time

// Phases:
// 1. Console app
// 2. Python script (plot trajectory & compare w/ analytical sln)
// 3. GUI (no rendering)
// 4. OpenGL window
// 5. GUI + OpenGL window

// Phase 1: Console app
// - User specified r0 (m), v0 (m/s), a0 (m/s^2), t_total (sec), dt (sec)
// - Handle user input robustly
// - Option to read from file (save typing)
// - Evolve system across t_total
// - Add sequential patterns
//   (1) Game Loop
//       - Make engine abstract class with render/step virtual functions
//       - Experiment with different parameter combinations
//   (2) Double Buffer
// - Show results in console
// - Output results to log

// Phase 2: Python app
// - Research ways of linking C++ / Python (pybind11 or SWIG)
// - Write Python script that plots actual trajectory vs. analytic solution
// - Experiment with different parameters and write reports
// - Resolve rounding errors (rf, vf actual != expected)

static Extractor* GetExtractor(const char arg);

int main()
{
	std::cout << "Welcome to the Physis (f-EYE-sis) Engine." << '\n';
	std::cout << "Running the client test app..." << '\n';

	InitialConditions ic;
	SystemConfig sc;
	std::cout << "Press C to enter initial conditions.  Press any other key to read from a file.\n";
	char arg;
	std::cin >> arg;

	Extractor* extractor = GetExtractor(arg);
	const auto system_parameters = extractor->Extract();
	delete extractor;

	ic = std::get<0>(system_parameters);
	sc = std::get<1>(system_parameters);

	std::cout << "\nInitial conditions:\n";
	std::cout << "r0 (m):\t\t(" << ic.r.X << ", " << ic.r.Y << ")\n";
	std::cout << "v0 (m/s):\t(" << ic.v.X << ", " << ic.v.Y << ")\n";
	std::cout << "a0 (m/s^2):\t(" << ic.a.X << ", " << ic.a.Y << ")\n";
	std::cout << "t_total (s):\t" << sc.total_time << '\n';  // TODO: Replace with sampling rate?
	std::cout << "dt (s):\t\t" << sc.delta_time << '\n';

	auto engine = new Engine(sc);
	engine->AddParticle(ic);
	engine->Run();
	delete engine;
}

static Extractor* GetExtractor(const char arg)
{
	if (arg == 'c' || arg == 'C')
	{
		return new ConsoleExtractor();
	}

	else
	{
		std::string filepath = GetValueFromPrompt<std::string>("Please enter the filename:\n");
		std::cout << "You entered: " << filepath << '\n';
		return new FileExtractor(filepath.c_str());
	}
}