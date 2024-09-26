#include "SystemState.h"

SystemState::SystemState()
	: m_system_curr_state(std::make_shared<ParticleSystem>()), 
	  m_system_prev_state(std::make_shared<ParticleSystem>())
{
}

void SystemState::AddParticle(const KinematicParameters& parameters)
{
    m_system_curr_state->Add(parameters);
    m_system_prev_state->Add(parameters);
}

std::shared_ptr<ParticleSystem> SystemState::GetCurrent()
{
	return m_system_curr_state;
}

std::shared_ptr<ParticleSystem> SystemState::GetPrevious()
{
	return m_system_prev_state;
}
