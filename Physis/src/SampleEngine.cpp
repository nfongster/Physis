#include "SampleEngine.h"

SampleEngine::SampleEngine(const TimeConfig& sc)
	: EngineBase(sc)
{
}

SampleEngine::~SampleEngine()
{
}

void SampleEngine::Render()
{
	std::cout << "Rendering...\n";
}

void SampleEngine::Interpolate(const double& factor)
{
	// TODO: Include or override base class interpolate?
}

void SampleEngine::AddParticle()
{
	m_system_state->AddParticle(InitialConditions());
}

void SampleEngine::AddParticle(const InitialConditions& ic)
{
	m_system_state->AddParticle(ic);
}

std::map<unsigned int, std::shared_ptr<Particle>> SampleEngine::Sample()
{
	return m_system_state->GetCurrent()->GetParticles();
}