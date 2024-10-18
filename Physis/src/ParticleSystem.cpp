#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
	m_particles(std::map<unsigned int, std::shared_ptr<Particle>>()),
	m_boundary(std::make_shared<Boundary>())
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

void ParticleSystem::Add(const KinematicParameters& parameters)
{
	m_particles.insert({ m_particles.size(), std::unique_ptr<Particle>(new Particle(parameters)) });
}

void ParticleSystem::AddBoundary(const Boundary& boundary)
{
	m_boundary = std::make_shared<Boundary>(boundary);
}

void ParticleSystem::Step(const double& dt)
{
	for (const auto& pair : m_particles)
	{
		pair.second->Step(dt);
		m_boundary->CheckCollision(pair.second);
	}
}

void ParticleSystem::Update(const unsigned int index, const KinematicParameters& parameters)
{
	if (index < 0 || index >= m_particles.size())
		throw std::out_of_range("Index out of range");

	m_particles[index] = std::make_shared<Particle>(parameters);
}

std::shared_ptr<Particle> ParticleSystem::operator[](const unsigned int index)
{
	if (index < 0 || index >= m_particles.size())
		throw std::out_of_range("Index out of range");
	
	return m_particles[index];
}

std::shared_ptr<Particle> ParticleSystem::GetParticleByIndex(unsigned int index)
{
	if (index < 0 || index >= m_particles.size())
		throw std::out_of_range("Index out of range");

	return m_particles[index];
}

std::vector<float> ParticleSystem::GetBoundaryPoints()
{
	return m_boundary->GetBoundaryPoints();
}
