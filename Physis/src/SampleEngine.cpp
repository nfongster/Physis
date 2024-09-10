#include "SampleEngine.h"

SampleEngine::SampleEngine(const TimeConfig& sc)
	: EngineBase(sc)
{
}

SampleEngine::~SampleEngine()
{
	delete m_system_state;
}

void SampleEngine::Update(const double& dt)
{
	m_system_state->GetCurrent()->Step(dt);
}

void SampleEngine::Render()
{
	std::cout << "Rendering...\n";
}

void SampleEngine::Interpolate(const double& factor)
{
	// Interpolate remaining accumulator time
}

void SampleEngine::AddParticle()
{
	m_system_state->GetCurrent()->Add(InitialConditions());
}

void SampleEngine::AddParticle(const InitialConditions& ic)
{
	m_system_state->GetCurrent()->Add(ic);
}

std::map<unsigned int, std::shared_ptr<Particle>> SampleEngine::Sample()
{
	return m_system_state->GetCurrent()->GetParticles();
}