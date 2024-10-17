#pragma once
#define _USE_MATH_DEFINES  // TODO: This is an old method for importing PI.  Considering upgrade to C++20 for std::numbers::pi.

#include <glew.h>
#include <memory>
#include <math.h>

#include "Physis.h"
#include "ShaderBuilder.h"
#include "EntityManager.h"

class CircleManager : public EntityManager
{
private:
	unsigned int m_ibo;
	std::map<unsigned int, std::shared_ptr<Particle>> m_vao_map;
	unsigned int m_shader_id;
	unsigned int m_num_vertices;
	unsigned int m_u_position_id;

public:
	CircleManager(const unsigned int num_segments);
	~CircleManager();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};

