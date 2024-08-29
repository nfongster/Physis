#pragma once
#include <glew.h>
#include <glfw3.h>
#include <math.h>
#include <map>

#include "Physis.h"
#include "ShaderBuilder.h"

class OpenGLEngine : public EngineBase
{
private:
	GLFWwindow* m_pWindow;
	unsigned int m_shader_id;
	unsigned int m_u_position_id;
	std::map<unsigned int, Particle*> m_vao_map;

	unsigned int POS_COORDS = 2;
	unsigned int NUM_VERTICES = 3;
	unsigned int NUM_TRIANGLES = 1;
	unsigned int NUM_TRIANGLE_CORNERS = 3;

protected:
	void OnStartup();
	bool ContinueLoop();
	void OnCompletion();

	void Update(const double& dt);
	void Render();
	void Interpolate(const double& factor);

public:
	OpenGLEngine(const SystemConfig& sc);
	~OpenGLEngine();

	void AddParticle();
	void AddParticle(const InitialConditions& ic);
};

