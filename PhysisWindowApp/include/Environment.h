#pragma once

#include <memory>

#include "Physis.h"
#include "ParticleManager.h"
#include "BoxManager.h"

class Environment
{
private:
	ParticleManager* m_entity_manager;
	BoxManager* m_boundary_manager;

public:
	Environment(ParticleManager* entity_manager, BoxManager* box_manager);
	~Environment();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};
