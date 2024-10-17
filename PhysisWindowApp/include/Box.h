#pragma once

#include "Entity.h"

class Box : public Entity
{
public:
	Box(float& height, float& width);
	~Box();

	void Render();
};