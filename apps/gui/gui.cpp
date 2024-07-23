#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"
#include "Particle.h"
#include "ParticleConfig.h"
#include "ParticleSystem.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    int success = glfwInit();
    
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Physica", nullptr, nullptr);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // float x0 = -0.5;
    // float v0 = 1;
    // ParticleConfig config = ParticleConfig(x0, v0, 0);
    // ParticleSystem* system = new ParticleSystem(config, 100, 1);

    Shader shader("apps/gui/shaders/vShader.glsl", "apps/gui/shaders/fShader.glsl");

    float xi_top = -0.50f;
    float xi_lft = -0.51f;
    float xi_rgt = -0.49f;
    
    float vertices[] = {
        xi_top, 0.02f, 0.0f,    0.0f, 0.0f, 0.0f,  // Top
        xi_lft, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f,  // Left
        xi_rgt, 0.0f, 0.0f,    0.0f, 0.0f, 0.0f   // Right
    };

    // Generate VBOs (for storing batches of vertices on the GPU) and VAOs (for managing them)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    unsigned int indices[] = {
        0, 1, 2
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    
    float dx = 0.0005; // step size per frame
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.42f, 0.41f, 0.37f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        // update locations
        if (vertices[0] < 0.5)
            vertices[0] += dx;
        if (vertices[6] < 0.49)
            vertices[6] += dx;
        if (vertices[12] < 0.51)
            vertices[12] += dx;
        // shader.setPos("vertexTop", xi_top, 0.02f);
        // shader.setPos("vertexLft", xi_lft, 0.00f);
        // shader.setPos("vertexRgt", xi_rgt, 0.00f);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_DYNAMIC_DRAW);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    // delete system;
    return 0;
}

/// @brief Function that is called when the window size changes.
/// @param window Window to resize
/// @param width New width of the window
/// @param height New height of the window
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void processInput(GLFWwindow *window)
{
    // Get Key will return either GLFW_PRESS or GLFW_RELEASE
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}