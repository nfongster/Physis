#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <chrono>
#include <iostream>
#include "shader_s.h"
#include "Particle.h"
#include "ParticleConfig.h"
#include "ParticleSystem.h"

const int SCR_WIDTH = 800;
const int SCR_HEIGHT = 600;

const float xi_top = -0.50f;
const float xi_lft = -0.51f;
const float xi_rgt = -0.49f;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);

int main()
{
    float v0;
    float a0;
    float dt;
    float t_total;

    std::cout << "Welcome to the physics simulator.\n";
    std::cout << "Please provide the starting parameters for the system:\n";

    std::cout << "(2) Initial velocity (x):\t";
    std::cin >> v0;  // suggested: 1.0

    std::cout << "(3) Acceleration (constant, x):\t";
    std::cin >> a0;  // suggested: 200

    std::cout << "(5) Total Time (s):\t\t";
    std::cin >> t_total;  // suggested: 100

    std::cout << "(6) Time Step (s):\t\t";
    std::cin >> dt;  // suggested: 0.0005

    ParticleConfig config = ParticleConfig(xi_top, v0, a0);
    ParticleSystem* system = new ParticleSystem(config, t_total, dt);

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

    Shader shader("apps/gui/shaders/vShader.glsl", "apps/gui/shaders/fShader.glsl");
    
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
    
    auto start = std::chrono::system_clock::now();

    // Accumulator tracks leftover simulation time that must carry over to the next frame
    // (i.e., if that leftover time is < dt)
    float accumulator = 0.0f;
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.42f, 0.41f, 0.37f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        // step and update locations
        auto end = std::chrono::system_clock::now();
        float frameTime = (end - start).count() * 1e-9f;  // in seconds
        start = end;
        accumulator += frameTime;

        if (system->is_running())
        {
            while (accumulator >= dt)
            {
                system->step(dt);
                float x = system->get_x();
                vertices[0] = x;
                vertices[6] = x - 0.01f;
                vertices[12] = x + 0.01f;

                accumulator -= dt;
            }
        }
        
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
    delete system;
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