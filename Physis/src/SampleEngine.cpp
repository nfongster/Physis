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
	// TODO: radius must be a variable
	m_system_state->AddParticle(KinematicParameters(), 0.015f);
}

void SampleEngine::AddParticle(const KinematicParameters& parameters)
{
	// TODO: radius must be a variable
	m_system_state->AddParticle(parameters, 0.015f);
}

std::map<unsigned int, std::shared_ptr<Particle>> SampleEngine::Sample()
{
	return m_system_state->GetCurrent()->GetParticles();
}