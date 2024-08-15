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