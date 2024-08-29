#include <glew.h>
#include <glfw3.h>
#include "OpenGLEngine.h"

int main()
{
    double t_total = 20;
    double dt = 0.0001;
    double scalar = 1;
    auto engine = new OpenGLEngine(SystemConfig(t_total, dt, scalar));

    Vec2 r0 (-0.45, -0.45);
    Vec2 v0 (0.1, 0.3);
    Vec2 a0 (0, -0.07);
    engine->AddParticle(InitialConditions(r0, v0, a0));
    engine->AddParticle(InitialConditions(Vec2(0, -0.3), Vec2(0.1, 0), Vec2()));
    engine->Run();
    delete engine;
    return 0;
}
