#pragma once
#include <glew.h>
#include <glfw3.h>
#include <sstream>
#include <fstream>
#include "Physis.h"

class OpenGLEngine : public EngineBase
{
private:
	GLFWwindow* m_window;
	unsigned int m_shader;

	unsigned int POS_COORDS = 2;
	unsigned int NUM_VERTICES = 6;
	unsigned int NUM_TRIANGLES = 4;
	unsigned int NUM_TRIANGLE_CORNERS = 3;

	std::string ParseShaderFile(const std::string& path);
	unsigned int BuildShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);
	unsigned int CompileShader(unsigned int type, const std::string& source);

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

