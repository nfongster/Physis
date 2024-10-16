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

class OpenGLEngine : public EngineBase
{
private:
	GLFWwindow* m_pWindow;
	EntityManager* m_entity_manager;

protected:
	void OnStartup();
	bool ContinueLoop();
	void OnCompletion();

	void Render();

public:
	OpenGLEngine(const TimeConfig& config, EntityManager* entity_manager);
	~OpenGLEngine();

	static std::unique_ptr<OpenGLEngine> WithTriangles(const TimeConfig& config);
	static std::unique_ptr<OpenGLEngine> WithCircles(const TimeConfig& config);

	void AddParticle();
	void AddParticle(const KinematicParameters& parameters);
};
