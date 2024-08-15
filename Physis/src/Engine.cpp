#include "Engine.h"

Engine::Engine(const SystemConfig& sc)
	: m_config(sc), m_system(new ParticleSystem())
{
}

Engine::~Engine()
{
	delete m_system;
}

void Engine::Update(const double& dt)
{
	m_system->Step(dt);
}

void Engine::Render()
{
	// Pause
}

void Engine::Interpolate(const double& factor)
{
	// Interpolate remaining accumulator time
}

void Engine::Run()
{
	// sec (simulation time)
	double t = 0;
	double accumulator = 0.0;
	const double dt_s = this->m_config.delta_time;
	auto start = std::chrono::high_resolution_clock::now();

	// TODO: add quit/cancel token instead of total-time
	while (t < this->m_config.total_time)
	{
		auto end = std::chrono::high_resolution_clock::now();
		// sec (simulation time)
		auto frameTime = (end - start).count() * this->m_config.time_scalar;
		start = end;
		accumulator += frameTime;

		while (accumulator >= dt_s)
		{
			this->Update(dt_s);
			t += dt_s;
			accumulator -= dt_s;
		}

		this->Interpolate(accumulator / dt_s);
		this->Render();
	}
}

void Engine::Pause()
{
	// pause thread
}

void Engine::Resume()
{
	// resume thread
}

void Engine::AddParticle()
{
	m_system->Add(InitialConditions());
}

void Engine::AddParticle(const InitialConditions& ic)
{
	m_system->Add(ic);
}

std::vector<Particle*> Engine::Sample()
{
	return m_system->GetParticles();
}