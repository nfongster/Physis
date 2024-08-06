#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Particle system can be constructed with default values")
{
	auto ps = ParticleSystem();
	auto particles = ps.GetParticles();
	REQUIRE(particles.size() == 0);
}

TEST_CASE("Particle system can be iterated with multiple particles")
{
	auto ps = ParticleSystem();
	int count = 5;

	for (int i = 0; i < count; i++)
		ps.Add(InitialConditions());

	REQUIRE(ps.GetParticles().size() == count);
	for (Particle* p : ps.GetParticles())
	{
		REQUIRE(p->GetPosition() == Vec2());
		REQUIRE(p->GetVelocity() == Vec2());
		REQUIRE(p->GetAcceleration() == Vec2());
	}
}

TEST_CASE("Particle system reliably stores particles' states in order of addition")
{
	auto ps = ParticleSystem();

	auto r01 = Vec2(1, 1);
	auto v01 = Vec2(3, -3);
	auto a01 = Vec2(-2, 5);
	auto ic1 = InitialConditions(r01, v01, a01);

	auto r02 = Vec2(-1, -1);
	auto v02 = Vec2(5, -5);
	auto a02 = Vec2(6, -7);
	auto ic2 = InitialConditions(r02, v02, a02);

	ps.Add(ic1);
	ps.Add(ic2);

	REQUIRE(ps.GetParticles().size() == 2);
	auto particles = ps.GetParticles();
	auto p1 = particles[0];
	auto p2 = particles[1];

	REQUIRE(p1->GetPosition() == r01);
	REQUIRE(p2->GetPosition() == r02);
	REQUIRE(p1->GetVelocity() == v01);
	REQUIRE(p2->GetVelocity() == v02);
	REQUIRE(p1->GetAcceleration() == a01);
	REQUIRE(p2->GetAcceleration() == a02);
}