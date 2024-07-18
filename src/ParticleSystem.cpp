#include "../include/ParticleSystem.h"

ParticleSystem::ParticleSystem(ParticleConfig config, float t_total, float dt)
    : particle(new Particle(config)), t_total(t_total), t_current(0.0), dt(dt) 
    { }

ParticleSystem::~ParticleSystem()
{
    delete this->particle;
}

void ParticleSystem::Execute()
{
    while (this->is_running())
    {
        this->particle->step(this->dt);
        this->t_current += this->dt;
    }
}

bool ParticleSystem::is_running()
{
    return this->t_current <= this->t_total;
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