#pragma once
#include <glew.h>
#include <glfw3.h>
#include <math.h>
#include <map>

#include "Physis.h"
#include "ShaderBuilder.h"
#include "EntityManager.h"

class OpenGLEngine : public EngineBase
{
private:
	GLFWwindow* m_pWindow;
	ParticleSystem* m_system_prev_state;
	EntityManager* m_entity_manager;

protected:
	void OnStartup();
	bool ContinueLoop();
	void OnCompletion();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

public:
	OpenGLEngine(const SystemConfig& sc, EntityManager* entity_manager);
	~OpenGLEngine();

	static std::unique_ptr<OpenGLEngine> WithTriangles(const SystemConfig& sc);

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};
