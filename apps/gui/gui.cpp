#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>

int main()
{
    int success = glfwInit();
    std::cout << success << '\n';
    std::cout << "This is a test\n";
    glfwTerminate();
}