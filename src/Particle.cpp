#include "../include/Particle.h"

Particle::Particle(ParticleConfig config) 
    : m_x(config.x0), m_v(config.v0), m_a(config.a0) 
    { }

void Particle::step(float dt)
{
    float x0 = this->m_x;
    float v0 = this->m_v;
    float a0 = this->m_a;

    float xf = x0 + v0 * dt + 0.5 * a0 * dt * dt;
    float vf = v0 + a0 * dt;

    this->m_x = xf;
    this->m_v = vf;
}

float Particle::get_x()
{
    return this->m_x;
}

float Particle::get_v()
{
    return this->m_v;
}

float Particle::get_a()
{
    return this->m_a;
}