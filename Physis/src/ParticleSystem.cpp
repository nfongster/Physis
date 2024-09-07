#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() : m_particles(std::map<unsigned int, std::shared_ptr<Particle>>())
{
}

ParticleSystem::~ParticleSystem()
{
	/*for (const auto& pair : m_particles)
		delete pair.second;*/
}

std::map<unsigned int, std::shared_ptr<Particle>> ParticleSystem::GetParticles()
{
	return m_particles;
}

void ParticleSystem::Add(const InitialConditions& ic)
{
	m_particles.insert({ m_particles.size(), std::unique_ptr<Particle>(new Particle(ic)) });
}

void ParticleSystem::Step(const double& dt)
{
	for (const auto& pair : m_particles)
		pair.second->Step(dt);
}

void ParticleSystem::Update(const unsigned int index, const InitialConditions& ic)
{
	if (index < 0 || index >= m_particles.size())
		throw std::out_of_range("Index out of range");

	m_particles[index] = std::make_shared<Particle>(ic);
}

std::shared_ptr<Particle> ParticleSystem::operator[](const unsigned int index)
{
	if (index < 0 || index >= m_particles.size())
		throw std::out_of_range("Index out of range");
	
	return m_particles[index];
}
