#include "SimpleEngine.h"

/// <summary>
/// Simple engine for benchmarking.
/// </summary>
/// <param name="sc"></param>
/// <param name="outdir"></param>
/// <param name="render_time">Time to pause (ms) during the render phase.</param>
SimpleEngine::SimpleEngine(const SystemConfig& sc, const std::string& outdir, 
	const std::chrono::duration<double, std::milli>& render_time)
	: EngineBase(sc), m_outdir(outdir), m_render_time(render_time)
{
}

SimpleEngine::~SimpleEngine()
{
	delete m_system;
}

void SimpleEngine::OnCompletion()
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

void SimpleEngine::Update(const double& dt)
{
	m_system->Step(dt);
}

void SimpleEngine::Render()
{
	std::this_thread::sleep_for(m_render_time);
	this->m_durations.push_back(m_duration);
}

void SimpleEngine::Interpolate(const double& factor)
{
	// Interpolate remaining accumulator time
}

void SimpleEngine::AddParticle()
{
	m_system->Add(InitialConditions());
}

void SimpleEngine::AddParticle(const InitialConditions& ic)
{
	m_system->Add(ic);
}
