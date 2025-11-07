#include "ShaderBuilder.h"

ShaderBuilder::ShaderBuilder(const std::string& directory)
	: m_shader_folder(directory)
{
}

unsigned int ShaderBuilder::Build()
{
    std::string vertexShaderSource = ParseShaderFile("VertexShader.shader");
    std::string fragmentShaderSource = ParseShaderFile("FragmentShader.shader");

    std::cout << vertexShaderSource << '\n';
    std::cout << fragmentShaderSource << '\n';

    return BuildShaderProgram(vertexShaderSource, fragmentShaderSource);
}

std::string ShaderBuilder::ParseShaderFile(const std::string& filename)
{   // Shader parsing/compiling ideas taken from Yan Chernikov
    std::ifstream stream(m_shader_folder + filename);
    std::string line;
    std::stringstream ss;

    while (std::getline(stream, line))
        ss << line << '\n';

    return ss.str();
}

unsigned int ShaderBuilder::BuildShaderProgram(const std::string& vertexSource, const std::string& fragmentSource)
{
    unsigned int program = glCreateProgram();
    unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexSource);
    unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentSource);

    glAttachShader(program, vs);
    glAttachShader(program, fs);
    glLinkProgram(program);
    glValidateProgram(program);

    glDeleteShader(vs);
    glDeleteShader(fs);

    return program;
}

unsigned int ShaderBuilder::CompileShader(unsigned int type, const std::string& source)
{
    unsigned int id = glCreateShader(type);
    const char* src = source.c_str();
    glShaderSource(id, 1, &src, nullptr);
    glCompileShader(id);

    int result;
    glGetShaderiv(id, GL_COMPILE_STATUS, &result);
    if (result == GL_FALSE)
    {
        int length;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
        char* message = (char*)alloca(length * sizeof(char));
        glGetShaderInfoLog(id, length, &length, message);
        std::cout << "Failed to compile " <<
            (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << " shader!" << std::endl;
        std::cout << message << std::endl;
        glDeleteShader(id);
        return 0;
    }

    return id;
}