#include <glew.h>
#include <glfw3.h>
#include <random>

#include "OpenGLEngine.h"

// Boundary
const double BOUNDARY_HALF_LENGTH = 0.995;

// Particle Geometry
const int NUM_CIRCLE_SEGMENTS = 10;
const float CIRCLE_RADIUS = 0.0001;//TODO: This does not change the rendered size, need to investigate
const int NUM_PARTICLES = 50;

// Particle Kinematics
const double MAX_INITIAL_SPEED = 20;
const Vec2 ACCELERATION = Vec2(0, -1.5);

static void RandomizeParticles(std::unique_ptr<OpenGLEngine>& engine)
{
    std::random_device rd;
    std::mt19937 generator(rd());

    std::uniform_real_distribution<double> position_dist(-BOUNDARY_HALF_LENGTH, BOUNDARY_HALF_LENGTH);
    std::uniform_real_distribution<double> velocity_dist(-MAX_INITIAL_SPEED, MAX_INITIAL_SPEED);

    for (int i = 0; i < NUM_PARTICLES; i++)
    {
        Vec2 r0(position_dist(generator), position_dist(generator));
        Vec2 v0(velocity_dist(generator), velocity_dist(generator));
        engine->AddParticle(KinematicParameters(r0, v0, ACCELERATION), CIRCLE_RADIUS);
    }
}

int main()
{
    auto t_total = std::chrono::duration<double>(50);
    auto dt = std::chrono::duration<double>(0.0005);
    double scalar = 1;
    std::vector<Vec2> polygon;
    polygon.push_back(Vec2(-BOUNDARY_HALF_LENGTH, -BOUNDARY_HALF_LENGTH));
    polygon.push_back(Vec2(-BOUNDARY_HALF_LENGTH, BOUNDARY_HALF_LENGTH));
    polygon.push_back(Vec2(BOUNDARY_HALF_LENGTH, BOUNDARY_HALF_LENGTH));
    polygon.push_back(Vec2(BOUNDARY_HALF_LENGTH, -BOUNDARY_HALF_LENGTH));

    // TODO: Clean up builder pattern
    auto engine = OpenGLEngine::WithCircles(TimeConfig(t_total, dt, scalar), NUM_CIRCLE_SEGMENTS);
    engine->AddBoundary(Boundary(polygon, 0.9));
    RandomizeParticles(engine);
    engine->Run();
    return 0;
}
