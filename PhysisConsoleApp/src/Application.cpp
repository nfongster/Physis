#include <iostream>
#include <Physis.h>
#include "Utility.h"
#include "Extractor.h"
#include "ConsoleExtractor.h"
#include "FileExtractor.h"

static Extractor* GetExtractor(const char arg);

static std::string OutputPath = "C:\\Code\\Physis\\PhysisConsoleApp\\";

static std::chrono::duration<double, std::milli> RenderDelay =
	std::chrono::duration<double, std::milli>(10);

int main()
{
	std::cout << "Welcome to the Physis (f-EYE-sis) Engine." << '\n';
	std::cout << "Running the client test app..." << '\n';

	KinematicParameters kp;
	TimeConfig config;
	int num_particles;

	std::cout << "Press C to enter initial conditions.  Press any other key to read from a file.\n";
	char arg;
	std::cin >> arg;

	Extractor* extractor = GetExtractor(arg);
	const auto system_parameters = extractor->Extract();
	delete extractor;

	kp = std::get<0>(system_parameters);
	config = std::get<1>(system_parameters);
	num_particles = std::get<2>(system_parameters);

	std::cout << "\nInitial conditions:\n";
	std::cout << "r0 (m):\t\t(" << kp.r.X << ", " << kp.r.Y << ")\n";
	std::cout << "v0 (m/s):\t(" << kp.v.X << ", " << kp.v.Y << ")\n";
	std::cout << "a0 (m/s^2):\t(" << kp.a.X << ", " << kp.a.Y << ")\n";
	std::cout << "t_total (s):\t" << config.total_time.count() << '\n';
	std::cout << "dt (s):\t\t" << config.delta_time.count() << '\n';
	std::cout << "t_scale (sim / comp):\t" << config.time_scalar << '\n';
	std::cout << "num particles:\t" << num_particles << '\n';

	auto engine = new BenchmarkEngine(config, OutputPath, RenderDelay);

	for (int i = 0; i < num_particles; i++)
		engine->AddParticle(kp);
	
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