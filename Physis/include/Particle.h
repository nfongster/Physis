#pragma once

#include <stdexcept>
#include "Core.h"
#include "Vec.h"
#include "KinematicParameters.h"
#include "Kinematics.h"

class PHYSIS_API Particle
{
private:
	Vec2 m_pos;
	Vec2 m_vel;
	Vec2 m_acc;
	float m_radius;

public:
	Particle();
	Particle(const KinematicParameters& parameters, const float& radius);
	~Particle();

	Vec2 GetPosition();
	Vec2 GetVelocity();
	Vec2 GetAcceleration();
	KinematicParameters GetKinematicParameters();
	float GetRadius();

	void Stop();
	void SwitchX(const float& elasticity, const float& x_reset);
	void SwitchY(const float& elasticity, const float& y_reset);
	void Step(const double& dt);
	void Interpolate(const std::shared_ptr<Particle> previousState, const double& factor);
};
