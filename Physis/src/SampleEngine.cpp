#include "SampleEngine.h"

SampleEngine::SampleEngine(const SystemConfig& sc)
	: EngineBase(sc)
{
}

SampleEngine::~SampleEngine()
{
	delete m_system;
}

void SampleEngine::Update(const double& dt)
{
	m_system->Step(dt);
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
	m_system->Add(InitialConditions());
}

void SampleEngine::AddParticle(const InitialConditions& ic)
{
	m_system->Add(ic);
}

std::map<unsigned int, Particle*> SampleEngine::Sample()
{
	return m_system->GetParticles();
}