#include <catch2/catch.hpp>
#include <Physis.h>
#include <iostream>
#include <chrono>

// Note: to run only on specific tags, do the following steps:
// 1. Go to project properties > debugging
// 2. Type tag(s) in command arguments field
TEST_CASE("Engine can be constructed and run", "[engine]")
{
	double t_total = 1;
	double dt_sim = 0.05;
	double scalar = 1.0;
	auto sc = TimeConfig(t_total, dt_sim, scalar);
	SampleEngine* engine = new SampleEngine(sc);
	engine->AddParticle();

	auto start = std::chrono::high_resolution_clock::now();
	engine->Run();
	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> duration = end - start;
	std::cout << "Time (ms): " << duration.count() << '\n';

	delete engine;
}