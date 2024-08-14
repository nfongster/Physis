#include "Engine.h"

Engine::Engine(const SystemConfig& sc)
	: m_config(sc), m_system(new ParticleSystem())
{
}

Engine::~Engine()
{
	delete m_system;
}

void Engine::Run()
{
	double t = 0;
	while (t < this->m_config.total_time)
	{
		m_system->Step(this->m_config.delta_time);
		t += this->m_config.delta_time;
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