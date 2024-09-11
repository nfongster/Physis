#include "SystemState.h"

SystemState::SystemState()
	: m_system_curr_state(new ParticleSystem()), m_system_prev_state(new ParticleSystem())
{
}

void SystemState::AddParticle(const InitialConditions& ic)
{
    m_system_curr_state->Add(ic);
    m_system_prev_state->Add(ic);
}

ParticleSystem* SystemState::GetCurrent()
{
	return m_system_curr_state;
}

ParticleSystem* SystemState::GetPrevious()
{
	return m_system_prev_state;
}
