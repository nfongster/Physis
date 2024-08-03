#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Particle default values are 0")
{
	auto p = Particle();
	auto r = p.GetPosition();
	auto v = p.GetVelocity();
	auto a = p.GetAcceleration();
	REQUIRE(r.X == 0);
	REQUIRE(v.X == 0);
	REQUIRE(a.X == 0);
	REQUIRE(r.Y == 0);
	REQUIRE(v.Y == 0);
	REQUIRE(a.Y == 0);
}

TEST_CASE("Particle injected values are expected")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-9.81, 0);
	auto ic = InitialConditions(r0, v0, a0);
	auto p = Particle(ic);

	REQUIRE(p.GetPosition().X == r0.X);
	REQUIRE(p.GetPosition().Y == r0.Y);

	REQUIRE(p.GetVelocity().X == v0.X);
	REQUIRE(p.GetVelocity().Y == v0.Y);

	REQUIRE(p.GetAcceleration().X == a0.X);
	REQUIRE(p.GetAcceleration().Y == a0.Y);
}

TEST_CASE("If dt = 0, then particle state is unchanged")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-9.81, 0);
	auto ic = InitialConditions(r0, v0, a0);
	auto p = Particle(ic);

	p.Step(0);

	REQUIRE(p.GetPosition().X == r0.X);
	REQUIRE(p.GetPosition().Y == r0.Y);

	REQUIRE(p.GetVelocity().X == v0.X);
	REQUIRE(p.GetVelocity().Y == v0.Y);

	REQUIRE(p.GetAcceleration().X == a0.X);
	REQUIRE(p.GetAcceleration().Y == a0.Y);
}