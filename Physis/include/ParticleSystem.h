#pragma once
#include "Particle.h"
#include "Boundary.h"

#include <map>
#include <memory>
#include <utility>

class PHYSIS_API ParticleSystem
{
private:
	std::map<unsigned int, std::shared_ptr<Particle>> m_particles;
	std::shared_ptr<Boundary> m_boundary;

public:
	ParticleSystem();
	~ParticleSystem();

	std::map<unsigned int, std::shared_ptr<Particle>> GetParticles();
	void Add(const KinematicParameters& parameters);
	void AddBoundary(const Boundary& boundary);
	void Step(const double& dt);
	void Update(const unsigned int index, const KinematicParameters& parameters);

	std::shared_ptr<Particle> operator[] (unsigned int index);
	std::shared_ptr<Particle> GetParticleByIndex(unsigned int index);
	std::vector<float> GetBoundaryPoints();
};
