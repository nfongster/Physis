#pragma once
#include "Vec.h"

class Particle
{
private:
	Vec2 m_pos;
	Vec2 m_vel;
	Vec2 m_acc;

public:
	Particle();
	~Particle();

	Vec2 GetPosition();
	Vec2 GetVelocity();
	Vec2 GetAcceleration();
};
