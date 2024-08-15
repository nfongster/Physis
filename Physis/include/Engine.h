#pragma once

#include "EngineBase.h"

class PHYSIS_API Engine : public EngineBase
{
private:
	SystemConfig m_config;
	ParticleSystem* m_system;

public:
	Engine(const SystemConfig& sc);
	~Engine();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

	void Run();
	void Pause();
	void Resume();

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
	std::vector<Particle*> Sample();
};