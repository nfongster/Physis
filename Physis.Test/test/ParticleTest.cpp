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