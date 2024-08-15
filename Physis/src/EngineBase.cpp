#include "EngineBase.h"

void EngineBase::Run()
{
	// sec (simulation time)
	double t = 0;
	double accumulator = 0.0;
	const double dt_s = this->m_config.delta_time;
	auto start = std::chrono::high_resolution_clock::now();

	// TODO: add quit/cancel token instead of total-time
	while (t < this->m_config.total_time)
	{
		auto end = std::chrono::high_resolution_clock::now();
		// sec (simulation time)
		auto frameTime = (end - start).count() * this->m_config.time_scalar;
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