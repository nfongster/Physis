import physis


class TestParticleSystem:
    def test_constructor(self):
        ps = physis.ParticleSystem()
        assert ps != None

    def test_get_particles(self):
        ps = physis.ParticleSystem()
        ps.add(physis.InitialConditions())
        pmap = ps.particles()
        assert pmap[0] != None
        assert len(pmap) == 1

    def test_update_particle(self):
        ps = physis.ParticleSystem()
        ps.add(physis.InitialConditions())

        r = physis.Vec2(2, -1)
        v = physis.Vec2(1, 1)
        a = physis.Vec2(-9, 0)
        ps.add(physis.InitialConditions(r, v, a))

        assert len(ps.particles()) == 2

        r2 = physis.Vec2(7, 8)
        v2 = physis.Vec2(-3, 4)
        a2 = physis.Vec2(0, 3)
        ps.update(0, physis.InitialConditions(r2, v2, a2))

        pmap = ps.particles()
        assert pmap[0].position().equals(r2)
        assert pmap[0].velocity().equals(v2)
        assert pmap[0].acceleration().equals(a2)

    def test_step_particle(self):
        ps = physis.ParticleSystem()
        v = physis.Vec2(1, 0)
        ps.add(physis.InitialConditions(physis.Vec2(), v, physis.Vec2()))
        ps.step(1)
        
        pmap = ps.particles()
        assert pmap[0].position().equals(physis.Vec2(1, 0))
        assert pmap[0].velocity().equals(v)
        assert pmap[0].acceleration().equals(physis.Vec2())