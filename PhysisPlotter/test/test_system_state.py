import physis


class TestSystemState:
    def test_constructor(self):
        ss = physis.SystemState()
        assert ss != None

    def test_add_particle(self):
        ss = physis.SystemState()
        ss.add(physis.InitialConditions())
        assert ss != None

    def test_get_current_state(self):
        ss = physis.SystemState()
        ss.add(physis.InitialConditions())
        pmap = ss.current().particles()
        assert pmap[0].position().equals(physis.Vec2())
        assert pmap[0].velocity().equals(physis.Vec2())
        assert pmap[0].acceleration().equals(physis.Vec2())

    def test_get_previous_state(self):
        ss = physis.SystemState()
        ss.add(physis.InitialConditions())
        pmap = ss.previous().particles()
        assert pmap[0].position().equals(physis.Vec2())
        assert pmap[0].velocity().equals(physis.Vec2())
        assert pmap[0].acceleration().equals(physis.Vec2())