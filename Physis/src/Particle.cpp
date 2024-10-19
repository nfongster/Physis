#include "Particle.h"

Particle::Particle() 
	: m_pos(Vec2(0, 0)), m_vel(Vec2(0, 0)), m_acc(Vec2(0, 0))
{
}

Particle::Particle(KinematicParameters parameters)
	: m_pos(parameters.r), m_vel(parameters.v), m_acc(parameters.a)
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

KinematicParameters Particle::GetKinematicParameters()
{
	return KinematicParameters(m_pos, m_vel, m_acc);
}

void Particle::Stop()
{
	m_vel = Vec2();
	m_acc = Vec2();
}

void Particle::SwitchX(const float& elasticity, const float& x_reset)
{
	m_pos = Vec2(x_reset, m_pos.Y);
	m_vel = Vec2(-m_vel.X * elasticity, m_vel.Y);
}

void Particle::SwitchY(const float& elasticity, const float& y_reset)
{
	m_pos = Vec2(m_pos.X, y_reset);
	m_vel = Vec2(m_vel.X, -m_vel.Y * elasticity);
}

void Particle::Step(const double& dt)
{
	if (dt < 0)
		throw std::invalid_argument("Input value was negative!");

	m_vel = Kinematics::UpdateVelocity(m_vel, m_acc, dt);
	m_pos = Kinematics::UpdatePosition(m_pos, m_vel, m_acc, dt);
}

void Particle::Interpolate(const std::shared_ptr<Particle> previousState, const double& factor)
{
	auto r_prev = previousState->GetPosition();
	auto v_prev = previousState->GetVelocity();
	auto a_prev = previousState->GetAcceleration();

	// CurrentState = CurrentState * factor + PreviousState * (1 - factor)
	auto r = m_pos * factor + r_prev * (1 - factor);
	auto v = m_vel * factor + v_prev * (1 - factor);
	auto a = m_acc * factor + a_prev * (1 - factor);

	m_pos = r;
	m_vel = v;
	m_acc = a;
}
