#pragma once
#include <glew.h>
#include <glfw3.h>
#include "Physis.h"

class OpenGLEngine : public EngineBase
{
private:
	GLFWwindow* m_window;
	unsigned int POS_COORDS = 2;
	unsigned int NUM_VERTICES = 6;

	unsigned int NUM_TRIANGLES = 4;
	unsigned int NUM_TRIANGLE_CORNERS = 3;

public:
	OpenGLEngine(const SystemConfig& sc);
	~OpenGLEngine();
	
	void OnStartup();
	bool ContinueLoop();
	void OnCompletion();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};

