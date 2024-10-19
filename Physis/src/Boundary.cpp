#include "Boundary.h"

Boundary::Boundary() : m_polygon(std::vector<Vec2>()), m_elasticity(0)
{
}

Boundary::Boundary(const std::vector<Vec2>& polygon, const float& elasticity) :
	m_polygon(polygon), m_elasticity(elasticity)
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
	Vec2 pos = particle->GetPosition();
	float r = particle->GetRadius();
	float top = m_polygon[1].Y * 1.5 - r;
	float bottom = m_polygon[0].Y / 2 + r;
	float left = m_polygon[0].X / 2 + r;
	float right = m_polygon[2].X * 1.5 - r;
	// TODO: Should give the particle mass, and the wall infinite mass.
	// TODO: make implementation not dependent on polygon point locations, add support for non-box shapes. etc.
	// TODO: add support for radius of particle (or if different shape, use polygon)

	// Perfectly Inelastic Collision (stop particle)
	if (m_elasticity == 0 && (pos.Y <= bottom || pos.X >= right || pos.X <= left || pos.Y >= top))
		particle->Stop();
	
	// Inelastic (<1) or Elastic (=1) Collision (reverse particle's velocity)
	// eventually, should replace elastic/inelastic collisions with a forcing function
	else
	{
		if (pos.Y < bottom)
			particle->SwitchY(m_elasticity, bottom);
		else if (pos.Y > top)
			particle->SwitchY(m_elasticity, top);
		if (pos.X > right)
			particle->SwitchX(m_elasticity, right);
		else if (pos.X < left)
			particle->SwitchX(m_elasticity, left);
	}
}
