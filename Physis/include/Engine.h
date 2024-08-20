#pragma once

#include "EngineBase.h"

class PHYSIS_API Engine : public EngineBase
{
public:
	Engine(const SystemConfig& sc);
	~Engine();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
	std::vector<Particle*> Sample();
};