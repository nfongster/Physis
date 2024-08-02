#include <iostream>
#include <fstream>
#include <sstream>
#include <Physis.h>

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

template <typename T>
static T GetValueFromPrompt(const char* prompt)
{
	T input{};
	std::cout << prompt;
	while (!(std::cin >> input))
	{
		std::cout << "Invalid input!\n";
		std::cin.clear();
		std::cin.ignore(1000, '\n');
		std::cout << prompt;
	}
	return input;
}

int main()
{
	std::cout << "Running the client test app..." << '\n';
	auto engine = new Engine();
	double r0x, r0y, v0x, v0y, a0x, a0y, t_total, dt;
	std::cout << "Press C to enter initial conditions.  Press any other key to read from a file.\n";
	char arg;
	std::cin >> arg;
	if (arg == 'c' || arg == 'C')
	{
		//auto init_conditions = consoleExtractor.Execute();
		std::cout << "Please enter the initial conditions:\n";

		r0x = GetValueFromPrompt<double>("r0.x (m):\t");
		r0y = GetValueFromPrompt<double>("r0.y (m):\t");
		v0x = GetValueFromPrompt<double>("v0.x (m/s):\t");
		v0y = GetValueFromPrompt<double>("v0.y (m/s):\t");
		a0x = GetValueFromPrompt<double>("a0.x (m/s^2):\t");
		a0y = GetValueFromPrompt<double>("a0.y (m/s^2):\t");
		t_total = GetValueFromPrompt<double>("t_total (s):\t");
		dt = GetValueFromPrompt<double>("dt (s):\t");
	}
	else
	{
		std::string filepath = GetValueFromPrompt<std::string>("Please enter the filename:\n");
		std::cout << "You entered: " << filepath << '\n';
		std::ifstream file(filepath);
		std::string line;
		int index;

		// Parse metadata
		std::getline(file, line);
		std::string token;
		std::stringstream ss = std::stringstream(line);
		while (std::getline(ss, token, ','))
		{
			if (token == "t") {
				std::getline(ss, token, ',');
				t_total = atof(token.c_str());
			}
			else if (token == "dt") {
				std::getline(ss, token, ',');
				dt = atof(token.c_str());
			}
		}
		std::getline(file, line);
		// TODO: parse header if we want to support different format types
		while (std::getline(file, line))
		{
			float* initial_conditions = new float[7];
			int i = 0;
			ss = std::stringstream(line);
			while (std::getline(ss, token, ','))
			{
				initial_conditions[i++] = atof(token.c_str());
			}
			index = (int)initial_conditions[0];
			r0x = initial_conditions[1];
			r0y = initial_conditions[2];
			v0x = initial_conditions[3];
			v0y = initial_conditions[4];
			a0x = initial_conditions[5];
			a0y = initial_conditions[6];
			delete[] initial_conditions;
		}
	}

	std::cout << "\Initial conditions:\n";
	std::cout << "r0 (m):\t\t(" << r0x << ", " << r0y << ")\n";
	std::cout << "v0 (m/s):\t(" << v0x << ", " << v0y << ")\n";
	std::cout << "a0 (m/s^2):\t(" << a0x << ", " << a0y << ")\n";
	std::cout << "t_total (s):\t" << t_total << '\n';
	std::cout << "dt (s):\t\t" << dt << '\n';

	engine->Run();
	delete engine;
}
