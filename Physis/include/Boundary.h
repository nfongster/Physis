#pragma once

#include <memory>
#include <vector>

#include "Vec.h"
#include "Particle.h"

class PHYSIS_API Boundary
{
private:
	const std::vector<Vec2> m_polygon;
	const float m_elasticity;

public:
	Boundary();
	Boundary(const std::vector<Vec2>& polygon, const float& elasticity = 0);
	~Boundary();

	std::vector<float> GetBoundaryPoints();
	void CheckCollision(std::shared_ptr<Particle> particle);
};

