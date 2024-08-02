#include "SystemState.h"

SystemState::SystemState() : m_particle(new Particle())
{
}

SystemState::~SystemState()
{
	delete m_particle;
}

Particle* SystemState::GetParticle()
{
	return m_particle;
}

void SystemState::Step(double dt)
{
	m_particle->Step(dt);
}
