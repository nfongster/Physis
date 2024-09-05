#pragma once

#include <stdexcept>
#include "Core.h"
#include "Vec.h"
#include "InitialConditions.h"
#include "Kinematics.h"

class PHYSIS_API Particle
{
private:
	Vec2 m_pos;
	Vec2 m_vel;
	Vec2 m_acc;

public:
	Particle();
	Particle(InitialConditions ic);
	~Particle();

	Vec2 GetPosition();
	Vec2 GetVelocity();
	Vec2 GetAcceleration();

	void SetPosition(Vec2 r);
	void SetVelocity(Vec2 v);
	void SetAcceleration(Vec2 a);

	void Step(double dt);
};
