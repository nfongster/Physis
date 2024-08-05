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

void Particle::Step(double dt)
{
	if (dt < 0)
		throw std::invalid_argument("Input value was negative!");

	m_pos.X += (m_vel.X * dt) + (0.5 * m_acc.X * dt * dt);
	m_pos.Y += (m_vel.Y * dt) + (0.5 * m_acc.Y * dt * dt);

	m_vel.X += m_acc.X * dt;
	m_vel.Y += m_acc.Y * dt;
}

//std::string Particle::PrintState()
//{
//	return std::string(m_pos.X) + '\t'
//		<< m_pos.Y << '\t'
//		<< m_vel.X << '\t'
//		<< m_vel.Y << '\t'
//		<< m_acc.X << '\t'
//		<< m_acc.Y << '\t' << '\n';
//}
