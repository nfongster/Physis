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
    int segments = 25;
    float radius = 0.015;
    auto engine = OpenGLEngine::WithCircles(TimeConfig(t_total, dt, scalar), 25);
    engine->AddBoundary(Boundary(polygon, 0.9));
    Vec2 r0(-len / 2, -len / 2);
    for (int i = 0; i < 3; i++)
    {
        Vec2 v0(0.1 + (i * 0.05), 0.6);
        Vec2 a0(0, -0.07 * (i + 1));
        engine->AddParticle(KinematicParameters(r0, v0, a0), radius);
    }
    engine->Run();
    return 0;
}
