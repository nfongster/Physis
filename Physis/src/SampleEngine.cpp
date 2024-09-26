#include "SampleEngine.h"

SampleEngine::SampleEngine(const TimeConfig& config)
	: EngineBase(config)
{
}

SampleEngine::~SampleEngine()
{
}

void SampleEngine::Render()
{
}

void SampleEngine::Interpolate(const double& factor)
{
	// TODO: Include or override base class interpolate?
}

void SampleEngine::AddParticle()
{
	m_system_state->AddParticle(KinematicParameters());
}

void SampleEngine::AddParticle(const KinematicParameters& parameters)
{
	m_system_state->AddParticle(parameters);
}

std::map<unsigned int, std::shared_ptr<Particle>> SampleEngine::Sample()
{
	return m_system_state->GetCurrent()->GetParticles();
}