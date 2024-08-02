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

enum DataType
{
	Position,
	Velocity,
	Acceleration, 
	TotalTime,
	DeltaTime
};

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
		std::string data;

		DataType state = DataType::Position;
		std::string index;
		while (std::getline(file, data, ','))
		{
			index = atoi(data.c_str());
			std::getline(file, data, ',');

			if (data == "r0")
				state = DataType::Position;
			else if (data == "v0")
				state = DataType::Velocity;
			else if (data == "a0")
				state = DataType::Acceleration;
			else if (data == "t")
				state = DataType::TotalTime;
			else if (data == "dt")
				state = DataType::DeltaTime;
						
			switch (state)
			{
				case DataType::Position:
					std::getline(file, data, ',');
					r0x = atof(data.c_str());
					std::getline(file, data);
					r0y = atof(data.c_str());
					break;

				case DataType::Velocity:
					std::getline(file, data, ',');
					v0x = atof(data.c_str());
					std::getline(file, data);
					v0y = atof(data.c_str());
					break;

				case DataType::Acceleration:
					std::getline(file, data, ',');
					a0x = atof(data.c_str());
					std::getline(file, data);
					a0y = atof(data.c_str());
					break;

				case DataType::TotalTime:
					std::getline(file, data);
					t_total = atof(data.c_str());
					break;

				case DataType::DeltaTime:
					std::getline(file, data);
					dt = atof(data.c_str());
					break;
			}
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
