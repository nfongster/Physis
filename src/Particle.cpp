#include "Particle.h"

float Particle::step_x(float dt)
{
    float x0 = this->m_x;
    float v0 = this->m_v;
    float a = this->m_a;
    return x0 + v0 * dt + 0.5 * a * dt * dt;
}

float Particle::step_v(float dt)
{
    float v0 = this->m_v;
    float a = this->m_a;
    return v0 + a * dt;
}