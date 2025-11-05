#include "ParticleSystem.h"

ParticleSystem::ParticleSystem() :
	m_particles(std::map<unsigned int, std::shared_ptr<Particle>>()),
	m_boundary(std::make_shared<Boundary>())
{
}

ParticleSystem::~ParticleSystem()
{
}

const std::map<unsigned int, std::shared_ptr<Particle>>& ParticleSystem::GetParticles() const
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
		m_boundary->CheckCollision(pair.second);
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
	Vec2 dr = p0->GetPosition() - p1->GetPosition();
	float radius0 = p0->GetRadius();
	float radius1 = p1->GetRadius();
	if (dr.Magnitude2() < (radius0 + radius1) * (radius0 + radius1))
		this->ResolveParticleCollisions(p0, p1);
}

void ParticleSystem::ResolveParticleCollisions(std::shared_ptr<Particle> p0, std::shared_ptr<Particle> p1)
{
	Vec2 dr = p0->GetPosition() - p1->GetPosition();
	Vec2 v0 = p0->GetVelocity();
	Vec2 v1 = p1->GetVelocity();
	Vec2 dv = v1 - v0;
	double C = dv.Dot(dr) / dr.Magnitude2();
	p0->SetVelocity(v0 + (dr * C));
	p1->SetVelocity(v1 - (dr * C));
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
