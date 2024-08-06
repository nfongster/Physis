#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Particle system can be constructed with default values")
{
	auto ps = ParticleSystem();
	auto p = ps.GetParticle();
	Vec2 r0 = p->GetPosition();
	Vec2 v0 = p->GetVelocity();
	Vec2 a0 = p->GetAcceleration();
	REQUIRE(r0.X == 0);
	REQUIRE(r0.Y == 0);
	REQUIRE(v0.X == 0);
	REQUIRE(v0.Y == 0);
	REQUIRE(a0.X == 0);
	REQUIRE(a0.Y == 0);
}