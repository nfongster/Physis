#pragma once

#include <glew.h>
#include <glfw3.h>
#include <math.h>
#include <memory>

#include "Physis.h"
#include "ShaderBuilder.h"
#include "EntityManager.h"

class TriangleManager : public EntityManager
{
private:
	unsigned int m_ibo;
	std::map<unsigned int, std::shared_ptr<Particle>> m_vao_map;
	unsigned int m_shader_id;
	unsigned int m_u_position_id;

	ParticleSystem* m_system;

	unsigned int POS_COORDS = 2;
	unsigned int NUM_VERTICES = 3;
	unsigned int NUM_TRIANGLES = 1;
	unsigned int NUM_TRIANGLE_CORNERS = 3;

public:
	TriangleManager(ParticleSystem* system);
	~TriangleManager();

	void Initialize();
	void Render();
};