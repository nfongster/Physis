#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : m_particles(std::vector<Particle*>())
{
}

ParticleSystem::~ParticleSystem()
{
	for (Particle* p : m_particles)
		delete p;
}

std::vector<Particle*> ParticleSystem::GetParticles()
{
	return m_particles;
}

void ParticleSystem::Add(const InitialConditions& ic)
{
	m_particles.push_back(new Particle(ic));
}

void ParticleSystem::Step(const double& dt)
{
	for (Particle* p : m_particles)
		p->Step(dt);
}
