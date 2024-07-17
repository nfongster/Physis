#pragma once
#include "Particle.h"
#include "ParticleConfig.h"

class ParticleSystem
{
private:
    float t_total;
    float t_current;
    Particle* particle;

public:
    ParticleSystem(ParticleConfig config, float t);
    ~ParticleSystem();
    void Execute();
    float get_x();
    float get_v();
    float get_a();
    float get_t();
    float displacement(float x0);
    float delta_v(float v0);
    float average_v(float x0);
};