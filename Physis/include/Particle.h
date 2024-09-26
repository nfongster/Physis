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

public:
	Particle();
	Particle(KinematicParameters parameters);
	~Particle();

	Vec2 GetPosition();
	Vec2 GetVelocity();
	Vec2 GetAcceleration();

	void Step(const double& dt);
	void Interpolate(const std::shared_ptr<Particle> previousState, const double& factor);
};
