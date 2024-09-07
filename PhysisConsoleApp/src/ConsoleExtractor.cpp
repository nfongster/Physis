#include "ConsoleExtractor.h"

ConsoleExtractor::ConsoleExtractor()
{
}

ConsoleExtractor::~ConsoleExtractor()
{
}

std::tuple<InitialConditions, TimeConfig, int> ConsoleExtractor::Extract()
{
	double r0x, r0y, v0x, v0y, a0x, a0y, t_total, dt, t_scale;
	int num_particles;
	// TODO: Support more than 1 particle (use while loop)
	std::cout << "Please enter the initial conditions:\n";

	r0x = GetValueFromPrompt<double>("r0.x (m):\t");
	r0y = GetValueFromPrompt<double>("r0.y (m):\t");
	v0x = GetValueFromPrompt<double>("v0.x (m/s):\t");
	v0y = GetValueFromPrompt<double>("v0.y (m/s):\t");
	a0x = GetValueFromPrompt<double>("a0.x (m/s^2):\t");
	a0y = GetValueFromPrompt<double>("a0.y (m/s^2):\t");
	t_total = GetValueFromPrompt<double>("t_total (s):\t");
	dt = GetValueFromPrompt<double>("dt (s):\t");
	t_scale = GetValueFromPrompt<double>("t_scale (sim / comp time):\t");
	num_particles = GetValueFromPrompt<double>("num_particles:\t");

	return
	{
		InitialConditions(Vec2(r0x, r0y), Vec2(v0x, v0y), Vec2(a0x, a0y)),
		TimeConfig(t_total, dt, t_scale),
		num_particles
	};
}
