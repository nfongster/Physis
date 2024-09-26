#include "EngineBase.h"

EngineBase::EngineBase(const TimeConfig& config)
	: m_config(config), m_system_state(std::make_shared<SystemState>())
{
}

void EngineBase::OnStartup()
{
	// Optional override
}

bool EngineBase::ContinueLoop()
{
	return m_current_time < m_config.total_time;
}

void EngineBase::OnCompletion()
{
	// Optional override
}

void EngineBase::Update(const double& dt)
{
	for (const auto& pair : m_system_state->GetCurrent()->GetParticles())
	{
		unsigned int index = pair.first;
		std::shared_ptr<Particle> particle = pair.second;
		KinematicParameters parameters = KinematicParameters(particle->GetPosition(), particle->GetVelocity(), particle->GetAcceleration());
		m_system_state->GetPrevious()->Update(index, parameters);
	}
	m_system_state->GetCurrent()->Step(dt);
}

void EngineBase::Interpolate(const double& factor)
{
	for (auto pair : m_system_state->GetCurrent()->GetParticles())
	{
		unsigned int i = pair.first;
		std::shared_ptr<Particle> p = pair.second;
		p->Interpolate((*m_system_state->GetPrevious())[i], factor);
	}
}

void EngineBase::Run()
{
	this->OnStartup();

	m_current_time = 0;
	double accumulator = 0.0;
	const double dt_s = this->m_config.delta_time;
	const double time_scalar = this->m_config.time_scalar;
	auto start = std::chrono::high_resolution_clock::now();

	while (this->ContinueLoop())
	{
		auto end = std::chrono::high_resolution_clock::now();
		this->m_duration = end - start;
		auto frameTime = this->m_duration.count() * 0.001 * time_scalar;
		start = end;
		accumulator += frameTime;

		while (accumulator >= dt_s)
		{
			this->Update(dt_s);
			m_current_time += dt_s;
			accumulator -= dt_s;
		}

		this->Interpolate(accumulator / dt_s);
		this->Render();
	}
	auto end = std::chrono::high_resolution_clock::now();
	this->m_duration = end - start;

	this->OnCompletion();
}

void EngineBase::Pause()
{
	// pause thread
}

void EngineBase::Resume()
{
	// resume thread
}