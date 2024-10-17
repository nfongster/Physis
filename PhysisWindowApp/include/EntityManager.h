#pragma once
class EntityManager
{
protected:
	unsigned int POS_COORDS = 2;

public:
	virtual void Initialize(std::shared_ptr<SystemState>& system_state) = 0;
	virtual void Render() = 0;
};

