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

void ParticleSystem::Add(const KinematicParameters& parameters, const float& radius)
{
	m_particles.insert({ m_particles.size(), std::make_shared<Particle>(parameters, radius) });
}

void ParticleSystem::AddBoundary(const Boundary& boundary)
{
	m_boundary = std::make_shared<Boundary>(boundary);
}

void ParticleSystem::Step(const double& dt)
{
	std::queue<std::shared_ptr<Particle>> q;
	for (const auto& pair : m_particles)
	{
		pair.second->Step(dt);
		m_boundary->CheckCollision(pair.second);  // rename to CheckBoundaryCollision
		q.push(pair.second);
	}
	
	while (!q.empty())
	{
		std::shared_ptr<Particle> p0 = q.front();
		q.pop();
		
		for (int i = 0; i < q.size(); i++)
		{
			std::shared_ptr<Particle> p1 = q.front();
			q.pop();
			this->CheckParticleCollisions(p0, p1);
			q.push(p1);
		}
	}
}

void ParticleSystem::CheckParticleCollisions(std::shared_ptr<Particle> p0, std::shared_ptr<Particle> p1)
{
	Vec2 r0 = p0->GetPosition();
	Vec2 r1 = p1->GetPosition();
	float radius0 = p0->GetRadius();
	float radius1 = p1->GetRadius();
	float distanceX = r0.X - r1.X;
	float distanceY = r0.Y - r1.Y;
	if ((distanceX * distanceX) + (distanceY * distanceY) <= (radius0 + radius1) * (radius0 + radius1))
	{
		p0->Stop();
		p1->Stop();
	}
}

void ParticleSystem::Update(const unsigned int index, const KinematicParameters& parameters)
{
	if (index < 0 || index >= m_particles.size())
		throw std::out_of_range("Index out of range");

	m_particles[index] = std::make_shared<Particle>(parameters, m_particles[index]->GetRadius());
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
