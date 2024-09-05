#include "Particle.h"

Particle::Particle() 
	: m_pos(Vec2(0, 0)), m_vel(Vec2(0, 0)), m_acc(Vec2(0, 0))
{
}

Particle::Particle(InitialConditions ic)
	: m_pos(ic.r), m_vel(ic.v), m_acc(ic.a)
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

void Particle::SetPosition(Vec2 r)
{
	m_pos = r;
}

void Particle::SetVelocity(Vec2 v)
{
	m_vel = v;
}

void Particle::SetAcceleration(Vec2 a)
{
	m_acc = a;
}

void Particle::Step(double dt)
{
	if (dt < 0)
		throw std::invalid_argument("Input value was negative!");

	m_pos = Kinematics::UpdatePosition(m_pos, m_vel, m_acc, dt);
	m_vel = Kinematics::UpdateVelocity(m_vel, m_acc, dt);
}
