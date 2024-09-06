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
	for (const auto& pair : ps.GetParticles())
	{
		REQUIRE(pair.second->GetPosition().Equals(Vec2()));
		REQUIRE(pair.second->GetVelocity().Equals(Vec2()));
		REQUIRE(pair.second->GetAcceleration().Equals(Vec2()));
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

	REQUIRE(p1->GetPosition().Equals(r01));
	REQUIRE(p2->GetPosition().Equals(r02));
	REQUIRE(p1->GetVelocity().Equals(v01));
	REQUIRE(p2->GetVelocity().Equals(v02));
	REQUIRE(p1->GetAcceleration().Equals(a01));
	REQUIRE(p2->GetAcceleration().Equals(a02));
}

TEST_CASE("Particle system can step 1 particle")
{
	auto ps = ParticleSystem();

	auto r0 = Vec2();
	auto v0 = Vec2(1, 1);
	auto a0 = Vec2(-1, 1);
	ps.Add(InitialConditions(r0, v0, a0));

	double dt = 0.1;
	ps.Step(dt);

	REQUIRE(ps.GetParticles()[0]->GetPosition()
		.Equals(Kinematics::UpdatePosition(r0, v0, a0, dt)));
	REQUIRE(ps.GetParticles()[0]->GetVelocity()
		.Equals(Kinematics::UpdateVelocity(v0, a0, dt)));
	REQUIRE(ps.GetParticles()[0]->GetAcceleration()
		.Equals(a0));
}

TEST_CASE("Particle system can step multiple particles")
{
	auto ps = ParticleSystem();

	auto r01 = Vec2();
	auto v01 = Vec2(1, 1);
	auto a01 = Vec2(-1, 1);
	ps.Add(InitialConditions(r01, v01, a01));

	auto r02 = Vec2(2, -3);
	auto v02 = Vec2(9, 3);
	auto a02 = Vec2(-7, 16);
	ps.Add(InitialConditions(r02, v02, a02));

	double dt = 0.1;
	ps.Step(dt);

	REQUIRE(ps.GetParticles()[0]->GetPosition()
		.Equals(Kinematics::UpdatePosition(r01, v01, a01, dt)));
	REQUIRE(ps.GetParticles()[0]->GetVelocity()
		.Equals(Kinematics::UpdateVelocity(v01, a01, dt)));
	REQUIRE(ps.GetParticles()[0]->GetAcceleration()
		.Equals(a01));

	REQUIRE(ps.GetParticles()[1]->GetPosition()
		.Equals(Kinematics::UpdatePosition(r02, v02, a02, dt)));
	REQUIRE(ps.GetParticles()[1]->GetVelocity()
		.Equals(Kinematics::UpdateVelocity(v02, a02, dt)));
	REQUIRE(ps.GetParticles()[1]->GetAcceleration()
		.Equals(a02));
}