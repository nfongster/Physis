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