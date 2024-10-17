#pragma once

#include <memory>

#include "Physis.h"
#include "EntityManager.h"

class Environment
{
private:
	EntityManager* m_entity_manager;

public:
	Environment(EntityManager* entity_manager);
	~Environment();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};
