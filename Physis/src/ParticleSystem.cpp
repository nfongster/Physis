#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : m_particles(std::map<unsigned int, Particle*>())
{
}

ParticleSystem::~ParticleSystem()
{
	for (const auto& pair : m_particles)
		delete pair.second;
}

std::map<unsigned int, Particle*> ParticleSystem::GetParticles()
{
	return m_particles;
}

void ParticleSystem::Add(const InitialConditions& ic)
{
	m_particles.insert({ m_particles.size(), new Particle(ic) });
}

void ParticleSystem::Step(const double& dt)
{
	for (const auto& pair : m_particles)
		pair.second->Step(dt);
}
