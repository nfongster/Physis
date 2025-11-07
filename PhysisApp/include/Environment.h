#pragma once

#include <memory>

#include "Physis.h"
#include "ParticleEntity.h"
#include "Box.h"

class Environment
{
private:
	ParticleEntity* m_particle;
	Box* m_boundary;

public:
	Environment(ParticleEntity* particle_entity, Box* box);
	~Environment();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};
