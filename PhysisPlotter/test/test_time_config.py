import physis


class TestTimeConfig:
    def test_default_constructor(self):
        tc = physis.TimeConfig()
        assert tc != None

    def test_params_constructor(self):
        tc = physis.TimeConfig(1.1, 0.9, 2.0)
        assert tc != None