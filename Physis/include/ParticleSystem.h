#pragma once
#include "Particle.h"

class PHYSIS_API ParticleSystem
{
private:
	Particle* m_particle;

public:
	ParticleSystem();
	~ParticleSystem();

	Particle* GetParticle();
	void Step(double dt);
};
