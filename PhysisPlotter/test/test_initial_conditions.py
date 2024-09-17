import physis


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