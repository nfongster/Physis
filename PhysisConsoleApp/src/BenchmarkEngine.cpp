#include "BenchmarkEngine.h"

/// <summary>
/// Simple engine for benchmarking.
/// </summary>
/// <param name="sc">System configuration.</param>
/// <param name="outdir">Directory to write results.</param>
/// <param name="render_time">Time to pause (ms) during the render phase.</param>
BenchmarkEngine::BenchmarkEngine(const SystemConfig& sc, const std::string& outdir, 
	const std::chrono::duration<double, std::milli>& render_time)
	: EngineBase(sc), m_outdir(outdir), m_render_time(render_time)
{
}

BenchmarkEngine::~BenchmarkEngine()
{
	delete m_system;
}

void BenchmarkEngine::OnCompletion()
{
	this->m_durations.push_back(m_duration);

	std::stringstream ss;
	ss << m_outdir << "results.txt";

	std::ofstream file;
	file.open(ss.str());

	for (auto duration : this->m_durations)
		file << duration.count() << '\n';
	
	file.close();
}

void BenchmarkEngine::Update(const double& dt)
{
	m_system->Step(dt);
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
	m_system->Add(InitialConditions());
}

void BenchmarkEngine::AddParticle(const InitialConditions& ic)
{
	m_system->Add(ic);
}
