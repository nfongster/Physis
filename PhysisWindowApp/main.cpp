#include <glew.h>
#include <glfw3.h>
#include "OpenGLEngine.h"

int main()
{
    auto t_total = std::chrono::duration<double>(20);
    auto dt = std::chrono::duration<double>(0.0005);
    double scalar = 1;
    auto engine = OpenGLEngine::WithTriangles(TimeConfig(t_total, dt, scalar));

    Vec2 r0 (-0.45, -0.45);
    for (int i = 0; i < 3; i++)
    {
        Vec2 v0(0.1 + (i * 0.05), 0.3);
        Vec2 a0(0, -0.07 * (i + 1));
        engine->AddParticle(KinematicParameters(r0, v0, a0));
    }
    engine->Run();
    return 0;
}
