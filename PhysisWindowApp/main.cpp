#include <glew.h>
#include <glfw3.h>
#include "OpenGLEngine.h"

int main()
{
    double t_total = 20;
    double dt = 0.0001;
    double scalar = 1;
    auto engine = OpenGLEngine::WithTriangles(SystemConfig(t_total, dt, scalar));

    Vec2 r0 (-0.45, -0.45);
    for (int i = 0; i < 3; i++)
    {
        Vec2 v0(0.1 + (i * 0.05), 0.3);
        Vec2 a0(0, -0.07 * (i + 1));
        engine->AddParticle(InitialConditions(r0, v0, a0));
    }
    engine->Run();
    return 0;
}
