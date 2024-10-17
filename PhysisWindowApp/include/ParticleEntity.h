#pragma once

#include <memory>

#include "Physis.h"
#include "Entity.h"

class ParticleEntity : public Entity
{
public:
	virtual void Initialize(std::shared_ptr<SystemState>& system_state) = 0;
};