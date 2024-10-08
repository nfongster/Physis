import physis
import pytest
from aggregator import *
from main import *


class TestAggregator:
    def test_constructor_with_valid_param(self):
        paths = {
            DataType.STABILITY : os.path.join("stability_test_file.txt")
        }
        aggregator = DataAggregator(paths)
        assert aggregator

    def test_constructor_with_null_param(self):
        with pytest.raises(ValueError):
            aggregator = DataAggregator(None)
        
    