#include <iostream>
#include <Physis.h>
#include "Utility.h"
#include "Extractor.h"
#include "ConsoleExtractor.h"
#include "FileExtractor.h"

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