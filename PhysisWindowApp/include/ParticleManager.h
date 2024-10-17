#pragma once

#include <memory>

#include "Physis.h"
#include "EntityManager.h"

class ParticleManager : public EntityManager
{
protected:
	unsigned int POS_COORDS = 2;

public:
	virtual void Initialize(std::shared_ptr<SystemState>& system_state) = 0;
};