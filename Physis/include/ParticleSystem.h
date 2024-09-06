#pragma once
#include "Particle.h"
#include <map>

class PHYSIS_API ParticleSystem
{
private:
	std::map<unsigned int, Particle*> m_particles;

public:
	ParticleSystem();
	~ParticleSystem();

	std::map<unsigned int, Particle*> GetParticles();
	void Add(const InitialConditions& ic);
	void Step(const double& dt);

	// TODO: add [] operator
};
