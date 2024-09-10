#include "BenchmarkEngine.h"

/// <summary>
/// Simple engine for benchmarking.
/// </summary>
/// <param name="sc">System configuration.</param>
/// <param name="outdir">Directory to write results.</param>
/// <param name="render_time">Time to pause (ms) during the render phase.</param>
BenchmarkEngine::BenchmarkEngine(const TimeConfig& sc, const std::string& outdir, 
	const std::chrono::duration<double, std::milli>& render_time)
	: EngineBase(sc), m_outdir(outdir), m_render_time(render_time)
{
}

BenchmarkEngine::~BenchmarkEngine()
{
	delete m_system_state;
}

void BenchmarkEngine::OnCompletion()
{
	this->m_durations.push_back(m_duration);

	std::stringstream ss;
	ss << m_outdir << "results.txt";

	std::ofstream file;
	file.open(ss.str());
	std::string buffer;
	int reservedBytes = 1 * 1024 * 1024;
	buffer.reserve(reservedBytes);

	for (auto duration : this->m_durations)
	{
		if (buffer.size() >= reservedBytes)
		{
			file.write(buffer.c_str(), buffer.size());
			buffer.clear();
		}
		buffer.append(std::to_string(duration.count()));
		buffer.append("\n");
	}
	if (!buffer.empty())
		file.write(buffer.c_str(), buffer.size());
	
	file.close();
}

void BenchmarkEngine::Update(const double& dt)
{
	m_system_state->GetCurrent()->Step(dt);
}

void BenchmarkEngine::Render()
{
	// Multiple render time by num particles?
	std::this_thread::sleep_for(m_render_time);
	this->m_durations.push_back(m_duration);
}

void BenchmarkEngine::Interpolate(const double& factor)
{
	// Interpolate remaining accumulator time
}

void BenchmarkEngine::AddParticle()
{
	m_system_state->GetCurrent()->Add(InitialConditions());
}

void BenchmarkEngine::AddParticle(const InitialConditions& ic)
{
	m_system_state->GetCurrent()->Add(ic);
}
