#include <glew.h>
#include <glfw3.h>
#include "OpenGLEngine.h"

int main()
{
    double t_total = 10;
    double dt = 0.1;
    double scalar = 1;
    auto engine = new OpenGLEngine(SystemConfig(t_total, dt, scalar));
    engine->Run();
    delete engine;
    return 0;
}
