#pragma once
class EntityManager
{
public:
	virtual void Initialize(std::shared_ptr<SystemState>& system_state) = 0;
	virtual void Render() = 0;
};

