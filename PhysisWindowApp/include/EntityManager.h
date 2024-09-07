#pragma once
class EntityManager
{
public:
	virtual void Initialize(ParticleSystem* particle_system) = 0;
	virtual void Render() = 0;
};

