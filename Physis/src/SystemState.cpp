#include "SystemState.h"

SystemState::SystemState()
	: m_system_curr_state(new ParticleSystem()), m_system_prev_state(new ParticleSystem())
{
}

ParticleSystem* SystemState::GetCurrent()
{
	return m_system_curr_state;
}

ParticleSystem* SystemState::GetPrevious()
{
	return m_system_prev_state;
}
