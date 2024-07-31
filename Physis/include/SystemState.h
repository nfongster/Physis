#pragma once
#include "Particle.h"

class SystemState
{
private:
	Particle* m_particle;

public:
	SystemState();
	~SystemState();

	Particle* GetParticle();
};
