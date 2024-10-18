#include "Boundary.h"

Boundary::Boundary() : m_polygon(std::vector<Vec2>())
{
}

Boundary::Boundary(const std::vector<Vec2>& polygon) : m_polygon(polygon)
{
}

Boundary::~Boundary()
{
}

std::vector<float> Boundary::GetBoundaryPoints()
{
	std::vector<float> vertices;
	for (auto p : m_polygon)
	{
		vertices.push_back(p.X);
		vertices.push_back(p.Y);
	}
	return vertices;
}

void Boundary::CheckCollision(std::shared_ptr<Particle> particle)
{
	// After stepping, check if particle has encountered a boundary
	// If so, either: stop it (perfectly inelastic), or reverse its velocity (elastic), or change its speed (inelastic)
	// Thus, later on, should specify an elasticity parameter, and give the particle mass, and the wall infinite mass.
}
