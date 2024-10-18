#pragma once

#include <memory>
#include <vector>

#include "Vec.h"
#include "Particle.h"

class PHYSIS_API Boundary
{
private:
	const std::vector<Vec2> m_polygon;

public:
	Boundary();
	Boundary(const std::vector<Vec2>& polygon);
	~Boundary();

	std::vector<float> GetBoundaryPoints();
	void CheckCollision(std::shared_ptr<Particle> particle);
};

