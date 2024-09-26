import physis


class TestSampleEngine:
    def test_constructor(self):
        engine = physis.SampleEngine(physis.TimeConfig())
        assert engine != None

    def test_add_particle_default(self):
        engine = physis.SampleEngine(physis.TimeConfig())
        engine.add()
        pmap = engine.sample()
        assert pmap[0] != None

    def test_add_particle_with_params(self):
        engine = physis.SampleEngine(physis.TimeConfig())
        r = physis.Vec2(1, 1)
        v = physis.Vec2(-0.1, 9.0)
        a = physis.Vec2(0, 2.1)
        engine.add(physis.KinematicParameters(r, v, a))
        pmap = engine.sample()
        assert pmap[0] != None

    def test_run_engine_no_particles_no_errors(self):
        engine = physis.SampleEngine(physis.TimeConfig())
        engine.run()

    def test_run_engine_1_particle_no_errors(self):
        engine = physis.SampleEngine(physis.TimeConfig())
        engine.add()
        engine.run()

    def test_run_engine_3_particles_finite_time_no_errors(self):
        engine = physis.SampleEngine(physis.TimeConfig(0.25, 0.01, 1))
        for i in range(3):
            engine.add()
        engine.run()