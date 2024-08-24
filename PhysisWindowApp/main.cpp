#include <glew.h>
#include <glfw3.h>
#include "OpenGLEngine.h"

int main()
{
    auto engine = new OpenGLEngine();
    engine->Run();
    delete engine;
    return 0;
}
