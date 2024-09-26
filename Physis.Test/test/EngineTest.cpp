#include <catch2/catch.hpp>
#include <Physis.h>

TEST_CASE("Engine can be constructed with no particles")
{
	SampleEngine* engine = new SampleEngine(TimeConfig());
	REQUIRE(engine != nullptr);
	REQUIRE(engine->Sample().size() == 0);
}

TEST_CASE("Engine can be constructed with a single particle with default values")
{
	TimeConfig sc = TimeConfig();
	SampleEngine* engine = new SampleEngine(sc);
	REQUIRE(engine != nullptr);

	engine->AddParticle();
	REQUIRE(engine->Sample().size() == 1);

	std::shared_ptr<Particle> p = engine->Sample()[0];
	REQUIRE(p->GetPosition().Equals(Vec2()));
	REQUIRE(p->GetVelocity().Equals(Vec2()));
	REQUIRE(p->GetAcceleration().Equals(Vec2()));
	delete engine;
}

TEST_CASE("Engine with 1 stationary particle can be sampled at a slower rate than dt")
{
	TimeConfig sc = TimeConfig();
	SampleEngine* engine = new SampleEngine(sc);
	engine->AddParticle();
	engine->Run();

	// TODO: Add sampling logic

	delete engine;
}

TEST_CASE("If engine runs with 1 stationary particle, then the particle's final and initial\
	conditions are the same")
{
	auto total_time = std::chrono::duration<double>(10);
	auto delta_time = std::chrono::duration<double>(1);

	SampleEngine* engine = new SampleEngine(TimeConfig(total_time, delta_time, 1));
	engine->AddParticle();
	engine->Run();

	std::shared_ptr<Particle> p = engine->Sample()[0];
	REQUIRE(p->GetPosition().Equals(Vec2()));
	REQUIRE(p->GetVelocity().Equals(Vec2()));
	REQUIRE(p->GetAcceleration().Equals(Vec2()));
}

TEST_CASE("If engine runs with 1 moving particle, then the particle's final conditions\
	match the analytic solution within a certain tolerance")
{	// TODO: Currently failing due to rounding errors.
	auto total_time = std::chrono::duration<double>(1);
	auto delta_time = std::chrono::duration<double>(0.0001);
	double tolerance = 1e-5;

	Vec2 r0 = Vec2(4, -2.3);
	Vec2 v0 = Vec2(6.5, 2.1);
	Vec2 a0 = Vec2(-2.2, 7.9);
	
	Vec2 rf = Kinematics::UpdatePosition(r0, v0, a0, total_time.count());
	Vec2 vf = Kinematics::UpdateVelocity(v0, a0, total_time.count());

	SampleEngine* engine = new SampleEngine(TimeConfig(total_time, delta_time, 1));
	engine->AddParticle(KinematicParameters(r0, v0, a0));
	engine->Run();

	std::shared_ptr<Particle> p = engine->Sample()[0];

	(std::cout << "rf (expected): " << rf) << std::endl;
	(std::cout << "rf (computed): " << p->GetPosition()) << std::endl;
	(std::cout << "vf (expected): " << vf) << std::endl;
	(std::cout << "vf (computed): " << p->GetVelocity()) << std::endl;

	REQUIRE(p->GetPosition().Equals(rf, tolerance));
	REQUIRE(p->GetVelocity().Equals(vf, tolerance));
	REQUIRE(p->GetAcceleration().Equals(a0, tolerance));
}