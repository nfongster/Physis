#include <iostream>
#include <fstream>
#include <sstream>
#include <Physis.h>
#include "FileExtractor.h"
#include "ConsoleExtractor.h"
#include "SystemConfig.h"
#include "Utility.h"

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
// - Show results in console
// - Output results to log

int main()
{
	std::cout << "Running the client test app..." << '\n';
	auto engine = new Engine();

	double r0x, r0y, v0x, v0y, a0x, a0y, t_total, dt;
	InitialConditions ic;
	SystemConfig sc;
	std::cout << "Press C to enter initial conditions.  Press any other key to read from a file.\n";
	char arg;
	std::cin >> arg;
	if (arg == 'c' || arg == 'C')
	{
		ConsoleExtractor extractor;
		const auto system_parameters = extractor.Extract();
		ic = std::get<0>(system_parameters);
		sc = std::get<1>(system_parameters);
	}
	else
	{
		std::string filepath = GetValueFromPrompt<std::string>("Please enter the filename:\n");
		std::cout << "You entered: " << filepath << '\n';

		FileExtractor extractor(filepath.c_str());
		const auto system_parameters = extractor.Extract();
		ic = std::get<0>(system_parameters);
		sc = std::get<1>(system_parameters);
	}

	std::cout << "\Initial conditions:\n";
	std::cout << "r0 (m):\t\t(" << ic.r.X << ", " << ic.r.Y << ")\n";
	std::cout << "v0 (m/s):\t(" << ic.v.X << ", " << ic.v.Y << ")\n";
	std::cout << "a0 (m/s^2):\t(" << ic.a.X << ", " << ic.a.Y << ")\n";
	std::cout << "t_total (s):\t" << sc.total_time << '\n';
	std::cout << "dt (s):\t\t" << sc.delta_time << '\n';

	engine->Run();
	delete engine;
}
