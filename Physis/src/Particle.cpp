#include "Particle.h"

Particle::Particle() : m_pos(Vec2(0, 0)), m_vel(Vec2(0, 0)), m_acc(Vec2(0, 0))
{
}

Particle::~Particle()
{
}

Vec2 Particle::GetPosition()
{
	return m_pos;
}

Vec2 Particle::GetVelocity()
{
	return m_vel;
}

Vec2 Particle::GetAcceleration()
{
	return m_acc;
}
