#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() 
	: m_particles(std::vector<Particle*>{ new Particle() })
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

void ParticleSystem::Step(double dt)
{
	for (Particle* p : m_particles)
		p->Step(dt);
}
