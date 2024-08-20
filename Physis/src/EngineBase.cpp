#include "EngineBase.h"

EngineBase::EngineBase(const SystemConfig& sc)
	: m_config(sc), m_system(new ParticleSystem())
{
}

void EngineBase::Run()
{
	double t = 0;
	double accumulator = 0.0;
	const double dt_s = this->m_config.delta_time;
	const double time_scalar = this->m_config.time_scalar;
	const double total_time = this->m_config.total_time;
	auto start = std::chrono::high_resolution_clock::now();

	// TODO: add quit/cancel token instead of total-time
	while (t < total_time)
	{
		auto end = std::chrono::high_resolution_clock::now();
		std::chrono::duration<double, std::milli> duration = end - start;
		auto frameTime = duration.count() * 0.001 * time_scalar;
		start = end;
		accumulator += frameTime;

		while (accumulator >= dt_s)
		{
			this->Update(dt_s);
			t += dt_s;
			accumulator -= dt_s;
		}

		this->Interpolate(accumulator / dt_s);
		this->Render();
	}
}

void EngineBase::Pause()
{
	// pause thread
}

void EngineBase::Resume()
{
	// resume thread
}