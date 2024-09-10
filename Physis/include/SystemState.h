#pragma once

#include "ParticleSystem.h"

class PHYSIS_API SystemState
{
private:
	ParticleSystem* m_system_curr_state;
	ParticleSystem* m_system_prev_state;

public:
	SystemState();
	ParticleSystem* GetCurrent();
	ParticleSystem* GetPrevious();
};

