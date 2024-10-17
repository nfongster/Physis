#pragma once
#define _USE_MATH_DEFINES  // TODO: This is an old method for importing PI.  Considering upgrade to C++20 for std::numbers::pi.

#include <glew.h>
#include <memory>
#include <math.h>

#include "Physis.h"
#include "ShaderBuilder.h"
#include "ParticleEntity.h"

class Circle : public ParticleEntity
{
private:
	unsigned int m_ibo;
	std::map<unsigned int, std::shared_ptr<Particle>> m_vao_map;
	unsigned int m_shader_id;
	unsigned int m_num_vertices;
	unsigned int m_u_position_id;

public:
	Circle(const unsigned int num_segments);
	~Circle();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};

