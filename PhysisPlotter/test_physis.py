import physis

class TestVec:
    def test_Vec1(self):
        v = physis.Vec1()
        vb = physis.Vec1()
        assert v.equals(vb)