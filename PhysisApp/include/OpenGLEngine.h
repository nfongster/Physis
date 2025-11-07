#pragma once
#include <glew.h>
#include <glfw3.h>
#include <math.h>
#include <map>

#include "Physis.h"

#include "Box.h"
#include "Circle.h"
#include "Entity.h"
#include "Environment.h"
#include "ShaderBuilder.h"
#include "Triangle.h"

class OpenGLEngine : public EngineBase
{
private:
	GLFWwindow* m_pWindow;
	Environment* m_environment;

protected:
	void OnStartup();
	bool ContinueLoop();
	void OnCompletion();

	void Render();

public:
	OpenGLEngine(const TimeConfig& config, Environment* environment);
	~OpenGLEngine();

	static std::unique_ptr<OpenGLEngine> WithTriangles(const TimeConfig& config);
	static std::unique_ptr<OpenGLEngine> WithCircles(const TimeConfig& config, const int& num_segments);
	
	void AddParticle();
	void AddParticle(const KinematicParameters& parameters, const float& radius);
	void AddBoundary(const Boundary& boundary);
};
