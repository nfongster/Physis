#pragma once
#include <glew.h>
#include <glfw3.h>
#include <math.h>
#include <map>

#include "Physis.h"
#include "ShaderBuilder.h"
#include "EntityManager.h"
#include "TriangleManager.h"
#include "CircleManager.h"
#include "Environment.h"

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
	OpenGLEngine(const TimeConfig& config, Environment* entity_manager);
	~OpenGLEngine();

	static std::unique_ptr<OpenGLEngine> WithTriangles(const TimeConfig& config);
	static std::unique_ptr<OpenGLEngine> WithCircles(const TimeConfig& config, const int& num_segments);
	
	void AddParticle();
	void AddParticle(const KinematicParameters& parameters);
};
