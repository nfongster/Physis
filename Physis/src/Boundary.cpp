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
	// Either: stop it (perfectly inelastic), or reverse its velocity (elastic), or change its speed (inelastic)
	// Thus, later on, should specify an elasticity parameter, and give the particle mass, and the wall infinite mass.

	Vec2 pos = particle->GetPosition();
	// TODO: make implementation not dependent on polygon point locations, add support for non-box shapes. etc.
	if (pos.Y <= m_polygon[0].Y / 2 || pos.X >= m_polygon[2].X / 2 || pos.X <= m_polygon[0].X / 2 || pos.Y >= m_polygon[1].Y / 2)
	{
		particle->Stop();
	}
}
