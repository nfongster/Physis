#pragma once
#include "ParticleConfig.h"

class Particle
{
private:
    float m_x;
    float m_v;
    float m_a;
    float m_t;

public:
    Particle(ParticleConfig config) 
        : m_x(config.x0), m_v(config.v0), m_a(config.a0), m_t(0.0) { }

    float step_x(float dt);
    float step_v(float dt);
};