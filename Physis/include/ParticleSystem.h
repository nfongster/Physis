#pragma once
#include "Particle.h"

class ParticleSystem
{
private:
	Particle* m_particle;

public:
	ParticleSystem();
	~ParticleSystem();

	Particle* GetParticle();
	void Step(double dt);
};
