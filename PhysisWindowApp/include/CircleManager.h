#pragma once

#include <glew.h>
#include <memory>

#include "Physis.h"
#include "EntityManager.h"

class CircleManager : public EntityManager
{
private:
	unsigned int m_shader_id;

public:
	CircleManager();
	~CircleManager();

	void Initialize(std::shared_ptr<SystemState>& system_state);
	void Render();
};

