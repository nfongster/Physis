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