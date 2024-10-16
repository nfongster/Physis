#include "EngineBase.h"

EngineBase::EngineBase(const TimeConfig& config) : 
	m_config(config), 
	m_current_frame_time(0), 
	m_elapsed_simulation_time(0),
	m_system_state(std::make_shared<SystemState>()) { }

void EngineBase::OnStartup() { /* Optional override */ }

void EngineBase::OnCompletion() { /* Optional override */ }

void EngineBase::Pause() { /* Optional override */ }

void EngineBase::Resume() { /* Optional override */ }

void EngineBase::Run()
{
	this->OnStartup();

	double accumulator_sec = 0.0;
	const double dt_sec = this->m_config.delta_time.count();
	const double time_scalar = this->m_config.time_scalar;
	auto start = std::chrono::high_resolution_clock::now();

	while (this->ContinueLoop())
	{
		auto end = std::chrono::high_resolution_clock::now();
		this->m_current_frame_time = end - start;
		auto frameTime = this->m_current_frame_time.count() * time_scalar;
		start = end;
		accumulator_sec += frameTime;
		double integration_time_sec = 0;

		while (accumulator_sec >= dt_sec)
		{
			this->Update(dt_sec);
			integration_time_sec += dt_sec;
			accumulator_sec -= dt_sec;
		}

		m_elapsed_simulation_time += std::chrono::duration<double>(integration_time_sec);
		this->Interpolate(accumulator_sec / dt_sec);
		this->Render();
	}
	auto end = std::chrono::high_resolution_clock::now();
	this->m_current_frame_time = end - start;

	this->OnCompletion();
}

bool EngineBase::ContinueLoop()
{
	return m_elapsed_simulation_time < m_config.total_time;
}

void EngineBase::Update(const double& dt_sec)
{
	for (const auto& pair : m_system_state->GetCurrent()->GetParticles())
	{
		unsigned int index = pair.first;
		std::shared_ptr<Particle> particle = pair.second;
		KinematicParameters parameters = particle->GetKinematicParameters();
		m_system_state->GetPrevious()->Update(index, parameters);
	}
	m_system_state->GetCurrent()->Step(dt_sec);
}

void EngineBase::Interpolate(const double& factor)
{
	for (auto& pair : m_system_state->GetCurrent()->GetParticles())
	{
		unsigned int i = pair.first;
		std::shared_ptr<Particle> p = pair.second;
		p->Interpolate(m_system_state->GetPrevious()->GetParticleByIndex(i), factor);
	}
}