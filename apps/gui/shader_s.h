#ifndef SHADER_H
#define SHADER_H

#include <glad/glad.h> // include glad to get all the required OpenGL headers
  
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


class Shader
{
public:
    // the program ID
    unsigned int ID;
  
    // constructor reads and builds the shader
    Shader(const char* vertexPath, const char* fragmentPath)
    {
        // 1. retrieve the vertex/fragment source code from filePath
        std::string vertexCode;
        std::string fragmentCode;
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;
        // ensure ifstream objects can throw exceptions:
        vShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        fShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
        try 
        {
            // open files
            vShaderFile.open(vertexPath);
            fShaderFile.open(fragmentPath);
            std::stringstream vShaderStream, fShaderStream;
            // read file's buffer contents into streams
            vShaderStream << vShaderFile.rdbuf();
            fShaderStream << fShaderFile.rdbuf();		
            // close file handlers
            vShaderFile.close();
            fShaderFile.close();
            // convert stream into string
            vertexCode   = vShaderStream.str();
            fragmentCode = fShaderStream.str();		
        }
        catch(std::ifstream::failure& e)
        {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESFULLY_READ" << std::endl;
            std::cout << e.what() << std::endl;
        }
        const char* vShaderCode = vertexCode.c_str();
        const char* triforceFragmentShaderCode = fragmentCode.c_str();
        // 2. compile shaders
        unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShader, 1, &vShaderCode, NULL);
        glCompileShader(vertexShader);
        checkCompileErrors(vertexShader, "VERTEX");

        // 2) CREATE THE FRAGMENT SHADER
        // Do the same thing as for the vertex shader.
        unsigned int triforceFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(triforceFragmentShader, 1, &triforceFragmentShaderCode, NULL);
        glCompileShader(triforceFragmentShader);
        checkCompileErrors(triforceFragmentShader, "TRIFORCEFRAGMENT");

        // unsigned int redFragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
        // glShaderSource(redFragmentShader, 1, &redFragmentShaderSource, NULL);
        // glCompileShader(redFragmentShader);
        // checkCompileErrors(redFragmentShader, "REDFRAGMENT");

        // 3) LINK THE SHADER PROGRAM
        // Simply create a shader program object, attach our shaders to it, then link them.
        ID = glCreateProgram();
        glAttachShader(ID, vertexShader);
        glAttachShader(ID, triforceFragmentShader);
        glLinkProgram(ID);
        checkLinkErrors(ID);

        // unsigned int shaderProgramRed = glCreateProgram();
        // glAttachShader(shaderProgramRed, vertexShader);
        // glAttachShader(shaderProgramRed, redFragmentShader);
        // glLinkProgram(shaderProgramRed);
        // checkLinkErrors(shaderProgramRed);

        glDeleteShader(vertexShader);
        glDeleteShader(triforceFragmentShader);
        // glDeleteShader(redFragmentShader);
    }
    // use/activate the shader
    void use()
    {
        glUseProgram(ID);
    }
    // utility uniform functions
    void setBool(const std::string &name, bool value) const
    {         
        glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value); 
    }
    void setInt(const std::string &name, int value) const
    { 
        glUniform1i(glGetUniformLocation(ID, name.c_str()), value); 
    }
    void setFloat(const std::string &name, float value) const
    { 
        glUniform1f(glGetUniformLocation(ID, name.c_str()), value); 
    }   

    void checkCompileErrors(unsigned int shaderId, const char* type)
    {
        char infoLog[512];
        int success;
        // Queries a shader for state information
        glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
        if (!success)
        {
            glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
            std::cout << "ERROR::SHADER::" << type << "::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }

    void checkLinkErrors(unsigned int programId)
    {
        char infoLog[512];
        int success;
        // Queries a program for state information
        glGetProgramiv(programId, GL_LINK_STATUS, &success);
        if (!success)
        {
            glGetProgramInfoLog(programId, 512, NULL, infoLog);
            std::cout << "ERROR::SHADERPROGRAM::COMPILATION_FAILED\n" << infoLog << std::endl;
        }
    }
};
  
#endif