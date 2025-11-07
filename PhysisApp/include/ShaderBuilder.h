#pragma once
#include <glew.h>
#include <glfw3.h>

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>

class ShaderBuilder
{
private:
	std::string m_shader_folder;

	std::string ParseShaderFile(const std::string& filename);
	unsigned int CompileShader(unsigned int type, const std::string& source);
	unsigned int BuildShaderProgram(const std::string& vertexSource, const std::string& fragmentSource);

public:
	ShaderBuilder(const std::string& directory);
	unsigned int Build();
};

