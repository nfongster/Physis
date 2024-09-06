#pragma once
#include "Particle.h"
#include <map>
#include <memory>
#include <utility>

class PHYSIS_API ParticleSystem
{
private:
	std::map<unsigned int, std::shared_ptr<Particle>> m_particles;

public:
	ParticleSystem();
	~ParticleSystem();

	std::map<unsigned int, std::shared_ptr<Particle>> GetParticles();
	void Add(const InitialConditions& ic);
	void Step(const double& dt);

	std::shared_ptr<Particle> operator[] (unsigned int index);
};
