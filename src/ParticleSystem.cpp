#include "ParticleSystem.h"

ParticleSystem::ParticleSystem(ParticleConfig config, float t_total)
    : particle(new Particle(config)), t_total(t_total), t_current(0.0) 
    { }

ParticleSystem::~ParticleSystem()
{
    delete this->particle;
}

void ParticleSystem::Execute()
{
    float dt = 0.1;  // todo: make configurable
    while (this->t_current <= this->t_total)
    {
        this->particle->step(dt);
        this->t_current += dt;
    }
}

float ParticleSystem::get_x()
{
    return this->particle->get_x();
}

float ParticleSystem::get_v()
{
    return this->particle->get_v();
}

float ParticleSystem::get_a()
{
    return this->particle->get_a();
}

float ParticleSystem::get_t()
{
    return this->t_current;
}

float ParticleSystem::displacement(float x0)
{
    float xf = this->particle->get_x();
    return xf - x0;
}

float ParticleSystem::delta_v(float v0)
{
    float vf = this->particle->get_v();
    return vf - v0;
}

float ParticleSystem::average_v(float x0)
{
    float dx = this->displacement(x0);
    float dt = this->t_current;
    return dx / dt;
}