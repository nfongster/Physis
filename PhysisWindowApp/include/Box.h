#pragma once

#include <glew.h>

#include "Physis.h"
#include "ShaderBuilder.h"
#include "Entity.h"

class Box : public Entity
{
private:
	unsigned int NUM_CORNERS = 4;

	unsigned int m_ibo;
	unsigned int m_vao;
	unsigned int m_shader;

public:
	Box();
	~Box();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};