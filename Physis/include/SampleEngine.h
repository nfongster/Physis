#pragma once

#include "EngineBase.h"

class PHYSIS_API SampleEngine : public EngineBase
{
public:
	SampleEngine(const SystemConfig& sc);
	~SampleEngine();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
	std::vector<Particle*> Sample();
};