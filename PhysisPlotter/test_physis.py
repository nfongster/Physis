import physis

class TestVec:
    def test_Vec1_default_equality(self):
        va = physis.Vec1()
        vb = physis.Vec1()
        assert va.equals(vb)

    def test_Vec1_param_equality(self):
        va = physis.Vec1(5)
        vb = physis.Vec1(5)
        assert va.equals(vb)

    def test_Vec2_default_equality(self):
        va = physis.Vec2()
        vb = physis.Vec2()
        assert va.equals(vb)

    def test_Vec2_param_equality(self):
        va = physis.Vec2(5, 7)
        vb = physis.Vec2(5, 7)
        assert va.equals(vb)

    def test_Vec3_default_equality(self):
        va = physis.Vec3()
        vb = physis.Vec3()
        assert va.equals(vb)

    def test_Vec3_param_equality(self):
        va = physis.Vec3(5, 7, 9)
        vb = physis.Vec3(5, 7, 9)
        assert va.equals(vb)


class TestInitialConditions:
    def test_default_constructor(self):
        ic = physis.InitialConditions()
        assert ic.r.equals(physis.Vec2())

    def test_param_constructor(self):
        r_exp = physis.Vec2(2, 1)
        v_exp = physis.Vec2(7, 8)
        a_exp = physis.Vec2(4, -1)
        ic = physis.InitialConditions(r_exp, v_exp, a_exp)
        assert ic.r.equals(r_exp)
        assert ic.v.equals(v_exp)
        assert ic.a.equals(a_exp)


class TestParticle:
    def test_default_constructor(self):
        v = physis.Vec2()
        p = physis.Particle()
        assert p.position().equals(v)
        assert p.velocity().equals(v)
        assert p.acceleration().equals(v)

    def test_param_constructor(self):
        r_exp = physis.Vec2(2, 1)
        v_exp = physis.Vec2(7, 8)
        a_exp = physis.Vec2(4, -1)
        ic = physis.InitialConditions(r_exp, v_exp, a_exp)
        p = physis.Particle(ic)
        assert p.position().equals(r_exp)
        assert p.velocity().equals(v_exp)
        assert p.acceleration().equals(a_exp)

    def test_step(self):
        ic = physis.InitialConditions(physis.Vec2(), physis.Vec2(1, 0), physis.Vec2())
        p = physis.Particle(ic)
        dt = 0.1
        rf_exp = physis.Vec2(0.1, 0)
        vf_exp = physis.Vec2(1, 0)
        af_exp = physis.Vec2()
        p.step(dt)
        assert p.position().equals(rf_exp)
        assert p.velocity().equals(vf_exp)
        assert p.acceleration().equals(af_exp)


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