#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "shader_s.h"

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

    Shader shader("apps/gui/shaders/vShader.glsl", "apps/gui/shaders/fShader.glsl");

    float triforceVertices[] = {
        0.0f, 0.75f, 0.0f,      0.0f, 0.0f, 1.0f,      // Top
        -0.375f, 0.0f, 0.0f,    0.0f, 1.0f, 0.0f,    // Left
        0.375f, 0.0f, 0.0f,     1.0f, 0.0f, 0.0f,     // Right
        -0.75f, -0.75f, 0.0f,   1.0f, 0.0f, 0.0f,   // Bottom-Left
        0.0f, -0.75f, 0.0f,     0.0f, 0.0f, 1.0f,     // Bottom
        0.75f, -0.75f, 0.0f,    0.0f, 1.0f, 0.0f,     // Bottom-Right
    };

    // Generate VBOs (for storing batches of vertices on the GPU) and VAOs (for managing them)
    unsigned int VBO, VAO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    unsigned int indices[] = {
        0, 1, 2,    // Top Triangle
        1, 3, 4,    // Left Triangle
        2, 4, 5     // Right Triangle
    };

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(triforceVertices), triforceVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    float verticesRed[] = {
        -0.375f, 0.5f, 0.0f,  // left
        -0.4f, 0.525f, 0.0f,
        -0.35f, 0.525f, 0.0f,
        0.375f, 0.5f, 0.0f,  // right
        0.4f, 0.525f, 0.0f,
        0.35f, 0.525f, 0.0f,
    };
   
    unsigned int VBOred, VAOred, EBOred;
    glGenVertexArrays(1, &VAOred);
    glGenBuffers(1, &VBOred);
    glGenBuffers(1, &EBOred);

    unsigned int indicesRed[] = {
        0, 1, 2,  // left
        3, 4, 5   // right
    };

    glBindVertexArray(VAOred);
    glBindBuffer(GL_ARRAY_BUFFER, VBOred);
    glBufferData(GL_ARRAY_BUFFER, sizeof(verticesRed), verticesRed, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOred);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicesRed), indicesRed, GL_STATIC_DRAW);
    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    
    while (!glfwWindowShouldClose(window))
    {
        processInput(window);
        glClearColor(0.42f, 0.41f, 0.37f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        shader.use();

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, 9, GL_UNSIGNED_INT, 0);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
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