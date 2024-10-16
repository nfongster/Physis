#include "CircleManager.h"

CircleManager::CircleManager()
{
}

CircleManager::~CircleManager()
{
	glDeleteProgram(m_shader_id);
}

void CircleManager::Initialize(std::shared_ptr<SystemState>& system_state)
{
}

void CircleManager::Render()
{
}
