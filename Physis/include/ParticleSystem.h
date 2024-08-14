#pragma once
#include "Particle.h"
#include <vector>

class PHYSIS_API ParticleSystem
{
private:
	std::vector<Particle*> m_particles;

public:
	ParticleSystem();
	~ParticleSystem();

	std::vector<Particle*> GetParticles();
	void Add(const InitialConditions& ic);
	void Step(const double& dt);
};
