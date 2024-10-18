#pragma once

#include <memory>
#include <vector>

#include "Vec.h"
#include "Particle.h"

class Boundary
{
private:
	const std::vector<Vec2> m_polygon;

public:
	Boundary();
	Boundary(const std::vector<Vec2>& polygon);
	~Boundary();

	void CheckCollision(std::shared_ptr<Particle> particle);
};

