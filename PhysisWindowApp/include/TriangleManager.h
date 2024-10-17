#pragma once

#include <glew.h>
#include <glfw3.h>
#include <math.h>
#include <memory>

#include "Physis.h"
#include "ShaderBuilder.h"
#include "ParticleManager.h"

class TriangleManager : public ParticleManager
{
private:
	unsigned int m_ibo;
	std::map<unsigned int, std::shared_ptr<Particle>> m_vao_map;
	unsigned int m_shader_id;
	unsigned int m_u_position_id;

	unsigned int NUM_VERTICES = 3;
	unsigned int NUM_TRIANGLE_CORNERS = 3;

public:
	TriangleManager();
	~TriangleManager();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};