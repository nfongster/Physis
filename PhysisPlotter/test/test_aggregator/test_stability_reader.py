import pytest
from aggregator import *

metadata = SimulationMetadata(1, timedelta(seconds=0), timedelta(seconds=0), timedelta(seconds=0), 1)


def test_stability_reader_constructor():
    reader = StabilityReader("stability.txt")
    assert reader

def test_stability_reader_cache_incomplete_filename():
    with pytest.raises(FileNotFoundError):
        reader = StabilityReader("badfilename.txt")
        reader.cache(metadata)

def test_stability_reader_cache_valid_filename():
    filename = "stability.txt"
    reader = StabilityReader(filename)
    with open(filename, 'w') as file:
        for data in [3.0, 5.0, 1.1, 12.3]:
            file.write(str(data) + '\n')
    reader.cache(metadata)

def test_stability_reader_write():
    filename = "stability.txt"
    reader = StabilityReader(filename)

    with open(filename, 'w') as file:
        for data in [3.0, 5.0, 1.1, 12.3]:
            file.write(str(data) + '\n')
    reader.cache(metadata)

    with sqlite3.Connection("result.db") as connection:
        reader.write(connection, "now", True, True)

def test_stability_reader_read():
    filename, data_array = "stability.txt", [3.0, 5.0, 1.1, 12.3]
    reader = StabilityReader(filename)
    with open(filename, 'w') as file:
        for data in data_array:
            file.write(str(data) + '\n')
    reader.cache(metadata)
    assert reader.times[metadata].tolist() == data_array

    with sqlite3.Connection("result.db") as connection:
        reader.write(connection, "now", True, True)

    assert not reader.times

    with sqlite3.Connection("result.db") as connection:
        reader.read(connection)

    assert reader.times[metadata].tolist() == data_array