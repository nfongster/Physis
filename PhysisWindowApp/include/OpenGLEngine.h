#pragma once
#include <glew.h>
#include <glfw3.h>
#include "Physis.h"

class OpenGLEngine : public EngineBase
{
public:
	OpenGLEngine();
	~OpenGLEngine();
	
	void OnStartup();
	void OnCompletion();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};

