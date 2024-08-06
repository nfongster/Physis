#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : m_particle(new Particle())
{
}

ParticleSystem::~ParticleSystem()
{
	delete m_particle;
}

Particle* ParticleSystem::GetParticle()
{
	return m_particle;
}

void ParticleSystem::Step(double dt)
{
	m_particle->Step(dt);
}
