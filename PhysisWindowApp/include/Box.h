#pragma once

#include <glew.h>

#include "Entity.h"

class Box : public Entity
{
private:
	float m_height;
	float m_width;
	unsigned int NUM_CORNERS = 4;

	unsigned int m_ibo;
	unsigned int m_vao;

public:
	Box(float& height, float& width);
	~Box();

	void Initialize();
	void Render();
};