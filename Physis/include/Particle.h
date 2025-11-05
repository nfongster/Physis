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
	Particle(const KinematicParameters& parameters, float radius);

	const Vec2& GetPosition() const;
	const Vec2& GetVelocity() const;
	const Vec2& GetAcceleration() const;
	KinematicParameters GetKinematicParameters();
	const float GetRadius() const;

	void Stop();
	void SwitchX(float elasticity, float x_reset);
	void SwitchY(float elasticity, float y_reset);
	void Step(double dt);
	void Interpolate(const std::shared_ptr<Particle> previousState, double factor);

	void SetVelocity(Vec2 vf);
};
