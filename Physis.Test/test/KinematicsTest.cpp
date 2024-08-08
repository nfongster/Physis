#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Position can be correctly updated for Vec1")
{
	Vec1 r0 = Vec1(2.1);
	Vec1 v0 = Vec1(-0.5);
	Vec1 a0 = Vec1(1.2);
	double dt = 0.8;

	Vec1 rf_computed = Kinematics::UpdatePosition(r0, v0, a0, dt);
	Vec1 rf_expected = Vec1(2.084);
	REQUIRE(rf_computed.Equals(rf_expected));
}

TEST_CASE("Position can be correctly updated for Vec2")
{
	Vec2 r0 = Vec2(2.1, -0.4);
	Vec2 v0 = Vec2(-0.5, 1.1);
	Vec2 a0 = Vec2(1.2, 0.5);
	double dt = 0.8;

	Vec2 rf_computed = Kinematics::UpdatePosition(r0, v0, a0, dt);
	Vec2 rf_expected = Vec2(2.084, 0.64);
	REQUIRE(rf_computed.Equals(rf_expected));
}

TEST_CASE("Position can be correctly updated for Vec3")
{
	Vec3 r0 = Vec3(2.1, -0.4, 0.7);
	Vec3 v0 = Vec3(-0.5, 1.1, 12.2);
	Vec3 a0 = Vec3(1.2, 0.5, -5.4);
	double dt = 0.8;

	Vec3 rf_computed = Kinematics::UpdatePosition(r0, v0, a0, dt);
	Vec3 rf_expected = Vec3(2.084, 0.64, 8.732);
	REQUIRE(rf_computed.Equals(rf_expected));
}

TEST_CASE("Velocity can be correctly updated for Vec1")
{
	Vec1 v0 = Vec1(-0.5);
	Vec1 a0 = Vec1(1.2);
	double dt = 0.8;

	Vec1 vf_computed = Kinematics::UpdateVelocity(v0, a0, dt);
	Vec1 vf_expected = Vec1(0.46);
	REQUIRE(vf_computed.Equals(vf_expected));
}

TEST_CASE("Velocity can be correctly updated for Vec2")
{
	Vec2 v0 = Vec2(-0.5, 1.1);
	Vec2 a0 = Vec2(1.2, 0.5);
	double dt = 0.8;

	Vec2 vf_computed = Kinematics::UpdateVelocity(v0, a0, dt);
	Vec2 vf_expected = Vec2(0.46, 1.5);
	REQUIRE(vf_computed.Equals(vf_expected));
}

TEST_CASE("Velocity can be correctly updated for Vec3")
{
	Vec3 v0 = Vec3(-0.5, 1.1, 12.2);
	Vec3 a0 = Vec3(1.2, 0.5, -5.4);
	double dt = 0.8;

	Vec3 vf_computed = Kinematics::UpdateVelocity(v0, a0, dt);
	Vec3 vf_expected = Vec3(0.46, 1.5, 7.88);
	REQUIRE(vf_computed.Equals(vf_expected));
}