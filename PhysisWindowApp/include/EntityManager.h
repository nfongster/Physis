#pragma once
class EntityManager
{
public:
	virtual void Initialize(SystemState* system_state) = 0;
	virtual void Render() = 0;
};

