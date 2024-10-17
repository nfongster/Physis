#pragma once

#include "EntityManager.h"

class BoxManager : public EntityManager
{
public:
	BoxManager(float& height, float& width);
	~BoxManager();

	void Render();
};