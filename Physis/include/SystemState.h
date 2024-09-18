#pragma once

#include "ParticleSystem.h"

class PHYSIS_API SystemState
{
private:
	std::shared_ptr<ParticleSystem> m_system_curr_state;
	std::shared_ptr<ParticleSystem> m_system_prev_state;

public:
	SystemState();
	void AddParticle(const InitialConditions& ic);
	std::shared_ptr<ParticleSystem> GetCurrent();
	std::shared_ptr<ParticleSystem> GetPrevious();
};

