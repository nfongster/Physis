#pragma once
#include "Physis.h"

class OpenGLEngine : public EngineBase
{
public:
	OpenGLEngine();
	~OpenGLEngine();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};

