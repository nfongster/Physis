#pragma once
#include "ParticleConfig.h"

class Particle
{
private:
    float m_x;
    float m_v;
    float m_a;

public:
    Particle(ParticleConfig config);
    void step(float dt);
    float get_x();
    float get_v();
    float get_a();
};