#pragma once

#include "BoundaryManager.h"

class BoxManager : public BoundaryManager
{
public:
	BoxManager(float& height, float& width);
	~BoxManager();

	void Render();
};