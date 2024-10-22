#include <glew.h>
#include <glfw3.h>

#include "OpenGLEngine.h"

int main()
{
    auto t_total = std::chrono::duration<double>(50);
    auto dt = std::chrono::duration<double>(0.0005);
    double scalar = 3;
    std::vector<Vec2> polygon;
    double len = 0.995;
    polygon.push_back(Vec2(-len, -len));
    polygon.push_back(Vec2(-len, len));
    polygon.push_back(Vec2(len, len));
    polygon.push_back(Vec2(len, -len));
    // TODO: Clean up builder pattern
    int segments = 10;
    float radius = 0.015;
    auto engine = OpenGLEngine::WithCircles(TimeConfig(t_total, dt, scalar), segments);
    engine->AddBoundary(Boundary(polygon, 0.9));

    // Projectile motion
    for (int i = 0; i < 3; i++)
    {
        Vec2 r0(-0.1, 0.1 * i);
        Vec2 v0(0.1 + (i * 0.05), 0.6);
        Vec2 a0(0, -0.07 * (i + 1));
        engine->AddParticle(KinematicParameters(r0, v0, a0), radius);
    }

    // Particle collisions
    Vec2 r0_p0(-len + radius, 0);
    Vec2 v0_p0(0.1, 0);
    Vec2 a0_p0;
    engine->AddParticle(KinematicParameters(r0_p0, v0_p0, a0_p0), radius);

    Vec2 r0_p1(len - radius, 0);
    Vec2 v0_p1(-0.2, 0);
    Vec2 a0_p1;
    engine->AddParticle(KinematicParameters(r0_p1, v0_p1, a0_p1), radius);

    engine->Run();
    return 0;
}
