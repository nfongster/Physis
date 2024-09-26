import physis


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
        ic = physis.KinematicParameters(r_exp, v_exp, a_exp)
        p = physis.Particle(ic)
        assert p.position().equals(r_exp)
        assert p.velocity().equals(v_exp)
        assert p.acceleration().equals(a_exp)

    def test_step(self):
        ic = physis.KinematicParameters(physis.Vec2(), physis.Vec2(1, 0), physis.Vec2())
        p = physis.Particle(ic)
        dt = 0.1
        rf_exp = physis.Vec2(0.1, 0)
        vf_exp = physis.Vec2(1, 0)
        af_exp = physis.Vec2()
        p.step(dt)
        assert p.position().equals(rf_exp)
        assert p.velocity().equals(vf_exp)
        assert p.acceleration().equals(af_exp)