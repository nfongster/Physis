#pragma once
#include "Particle.h"
#include "ParticleConfig.h"

class ParticleSystem
{
private:
    Particle* particle;
    float t_total;
    float t_current;
    float dt;

public:
    float* x_buffer;
    float* v_buffer;

public:
    ParticleSystem(ParticleConfig config, float t_total, float dt);
    ~ParticleSystem();

    void execute();
    bool is_running();

    float get_x();
    float get_v();
    float get_a();
    float get_t();
    float displacement(float x0);
    float delta_v(float v0);
    float average_v(float x0);
};