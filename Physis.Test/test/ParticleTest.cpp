#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Particle default values are 0")
{
	auto p = Particle();
	auto r = p.GetPosition();
	auto v = p.GetVelocity();
	auto a = p.GetAcceleration();
	REQUIRE(r == Vec2());
	REQUIRE(v == Vec2());
	REQUIRE(a == Vec2());
}

TEST_CASE("Particle injected values are expected")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-9.81, 0);
	auto ic = InitialConditions(r0, v0, a0);
	auto p = Particle(ic);

	REQUIRE(p.GetPosition() == r0);
	REQUIRE(p.GetVelocity() == v0);
	REQUIRE(p.GetAcceleration() == a0);
}

TEST_CASE("If dt = 0, then particle state is unchanged")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-9.81, 0);
	auto ic = InitialConditions(r0, v0, a0);
	auto p = Particle(ic);

	p.Step(0);

	REQUIRE(p.GetPosition() == r0);
	REQUIRE(p.GetVelocity() == v0);
	REQUIRE(p.GetAcceleration() == a0);
}

TEST_CASE("If dt > 0, then particle state is correctly updated")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-1, 1);
	auto ic = InitialConditions(r0, v0, a0);
	auto p = Particle(ic);

	double dt = 1;
	p.Step(dt);

	// xf = x0 + v0 * dt + 0.5 * a * dt^2
	double xf = r0.X + v0.X * dt + 0.5 * a0.X * dt * dt;
	double yf = r0.Y + v0.Y * dt + 0.5 * a0.Y * dt * dt;
	REQUIRE(p.GetPosition() == Vec2(xf, yf));

	// vf = v0 + a * dt
	double vxf = v0.X + a0.X * dt;
	double vyf = v0.Y + a0.Y * dt;
	REQUIRE(p.GetVelocity() == Vec2(vxf, vyf));

	// Constant acceleration
	REQUIRE(p.GetAcceleration() == a0);
}

TEST_CASE("If dt < 0, then exception is thrown")
{
	auto r0 = Vec2(1, 1);
	auto v0 = Vec2(1, 0);
	auto a0 = Vec2(-1, 1);
	auto ic = InitialConditions(r0, v0, a0);
	auto p = Particle(ic);

	double dt = -1;
	REQUIRE_THROWS_AS(p.Step(dt), std::invalid_argument);
}