#pragma once

#include <memory>

#include "Physis.h"
#include "EntityManager.h"
#include "BoundaryManager.h"

class Environment
{
private:
	EntityManager* m_entity_manager;
	BoundaryManager* m_boundary_manager;

public:
	Environment(EntityManager* entity_manager, BoundaryManager* box_manager);
	~Environment();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};
