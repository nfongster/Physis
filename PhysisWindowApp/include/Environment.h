#pragma once

#include <memory>

#include "Physis.h"
#include "ParticleEntity.h"
#include "Box.h"

class Environment
{
private:
	ParticleEntity* m_entity_manager;
	Box* m_boundary_manager;

public:
	Environment(ParticleEntity* entity_manager, Box* box_manager);
	~Environment();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};
