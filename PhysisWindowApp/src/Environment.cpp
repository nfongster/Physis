#include "Environment.h"

Environment::Environment(EntityManager* entity_manager, BoundaryManager* boundary_manager) 
	: m_entity_manager(entity_manager), m_boundary_manager(boundary_manager)
{
}

Environment::~Environment()
{
	delete m_entity_manager;
	delete m_boundary_manager;
}

void Environment::Initialize(std::shared_ptr<SystemState>& system_state)
{
	m_entity_manager->Initialize(system_state);
}

void Environment::Render()
{
	m_boundary_manager->Render();
	m_entity_manager->Render();
}
