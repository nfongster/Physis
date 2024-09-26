#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Particle default values are 0")
{
	auto p = Particle();
	auto r = p.GetPosition();
	auto v = p.GetVelocity();
	auto a = p.GetAcceleration();
	REQUIRE(r.Equals(Vec2()));
	REQUIRE(v.Equals(Vec2()));
	REQUIRE(a.Equals(Vec2()));
}

TEST_CASE("Particle injected values are expected")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-9.81, 0);
	auto parameters = KinematicParameters(r0, v0, a0);
	auto p = Particle(parameters);

	REQUIRE(p.GetPosition().Equals(r0));
	REQUIRE(p.GetVelocity().Equals(v0));
	REQUIRE(p.GetAcceleration().Equals(a0));
}

TEST_CASE("If dt = 0, then particle state is unchanged")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-9.81, 0);
	auto parameters = KinematicParameters(r0, v0, a0);
	auto p = Particle(parameters);

	p.Step(0);

	REQUIRE(p.GetPosition().Equals(r0));
	REQUIRE(p.GetVelocity().Equals(v0));
	REQUIRE(p.GetAcceleration().Equals(a0));
}

TEST_CASE("If dt > 0, then particle state is correctly updated")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-1, 1);
	auto parameters = KinematicParameters(r0, v0, a0);
	auto p = Particle(parameters);

	double dt = 1;
	p.Step(dt);

	REQUIRE(p.GetPosition().Equals(Kinematics::UpdatePosition(r0, v0, a0, dt)));
	REQUIRE(p.GetVelocity().Equals(Kinematics::UpdateVelocity(v0, a0, dt)));
	REQUIRE(p.GetAcceleration().Equals(a0));
}

TEST_CASE("If dt < 0, then exception is thrown")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-1, 1);
	auto parameters = KinematicParameters(r0, v0, a0);
	auto p = Particle(parameters);

	double dt = -1;
	REQUIRE_THROWS_AS(p.Step(dt), std::invalid_argument);
}