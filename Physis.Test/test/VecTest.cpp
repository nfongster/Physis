#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Vec1 default value is 0")
{
	Vec1 v;
	REQUIRE(v.X == 0);
}

TEST_CASE("Vec1 injected value is expected")
{
	double x = 10.0;
	Vec1 v(x);
	REQUIRE(v.X == x);
}

TEST_CASE("Vec2 default value is (0, 0)")
{
	Vec2 v;
	REQUIRE(v.X == 0);
	REQUIRE(v.Y == 0);
}

TEST_CASE("Vec2 injected values are expected")
{
	double x = 10.0;
	double y = -10.0;
	Vec2 v(x, y);
	REQUIRE(v.X == x);
	REQUIRE(v.Y == y);
}

TEST_CASE("Vec3 default value is (0, 0, 0)")
{
	Vec3 v;
	REQUIRE(v.X == 0);
	REQUIRE(v.Y == 0);
	REQUIRE(v.Z == 0);
}

TEST_CASE("Vec3 injected values are expected")
{
	double x = 10.0;
	double y = -10.0;
	double z = 0.1;
	Vec3 v(x, y, z);
	REQUIRE(v.X == x);
	REQUIRE(v.Y == y);
	REQUIRE(v.Z == z);
}

TEST_CASE("Two identical Vec1's are equal")
{
	Vec1 a = Vec1(3);
	Vec1 b = Vec1(3);
	REQUIRE(a == b);
}

TEST_CASE("Two identical Vec2's are equal")
{
	Vec2 a = Vec2(3, 4);
	Vec2 b = Vec2(3, 4);
	REQUIRE(a == b);
}

TEST_CASE("Two identical Vec3's are equal")
{
	Vec3 a = Vec3(3, 4, -8);
	Vec3 b = Vec3(3, 4, -8);
	REQUIRE(a == b);
}

TEST_CASE("Two different Vec1's are not equal")
{
	Vec1 a = Vec1(3);
	Vec1 b = Vec1(2);
	REQUIRE(a != b);
}

TEST_CASE("Two different Vec2's are not equal")
{
	Vec2 a = Vec2(3, 4);
	Vec2 b = Vec2(3, 5);
	REQUIRE(a != b);
}

TEST_CASE("Two different Vec3's are not equal")
{
	Vec3 a = Vec3(3, 4, -8);
	Vec3 b = Vec3(3, 3, -8);
	REQUIRE(a != b);
}