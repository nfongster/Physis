#include "Environment.h"

Environment::Environment(ParticleEntity* particle_entity, Box* box)
	: m_particle(particle_entity), m_boundary(box)
{
}

Environment::~Environment()
{
	delete m_particle;
	delete m_boundary;
}

void Environment::Initialize(std::shared_ptr<SystemState>& system_state)
{
	m_boundary->Initialize();
	m_particle->Initialize(system_state);
}

void Environment::Render()
{
	m_boundary->Render();
	m_particle->Render();
}
