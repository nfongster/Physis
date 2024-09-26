#pragma once

#include "EngineBase.h"

class PHYSIS_API SampleEngine : public EngineBase
{
public:
	SampleEngine(const TimeConfig& config);
	~SampleEngine();

	void Render();
	void Interpolate(const double& factor);

	void AddParticle();
	void AddParticle(const KinematicParameters& parameters);
	std::map<unsigned int, std::shared_ptr<Particle>> Sample();
};