import pytest, time
from aggregator import *

metadata = SimulationMetadata(1, timedelta(seconds=0), timedelta(seconds=0), timedelta(seconds=0), 1)
dbname = "result.db"


def test_constructor():
    reader = StabilityReader("stability.txt")
    assert reader

def test_cache_incomplete_filename():
    with pytest.raises(FileNotFoundError):
        reader = StabilityReader("badfilename.txt")
        reader.cache(metadata, build_timestamp_str())

def test_cache_valid_filename():
    filename, data_array, timestamp = "stability.txt", [3.0, 5.0, 1.1, 12.3], build_timestamp_str()
    reader = StabilityReader(filename)

    with open(filename, 'w') as file:
        for data in data_array:
            file.write(str(data) + '\n')
    reader.cache(metadata, timestamp)

def test_write():
    filename, data_array, timestamp = "stability.txt", [3.0, 5.0, 1.1, 12.3], build_timestamp_str()
    reader = StabilityReader(filename)

    with open(filename, 'w') as file:
        for data in data_array:
            file.write(str(data) + '\n')
    reader.cache(metadata, timestamp)

    with sqlite3.Connection(dbname) as connection:
        reader.write(connection, True, True)

def test_read():
    filename, data_array, timestamp = "stability.txt", [3.0, 5.0, 1.1, 12.3], build_timestamp_str()
    reader = StabilityReader(filename)

    with open(filename, 'w') as file:
        for data in data_array:
            file.write(str(data) + '\n')
    reader.cache(metadata, timestamp)
    assert reader.times[timestamp].times.tolist() == data_array

    with sqlite3.Connection(dbname) as connection:
        reader.write(connection, True, True)
    assert not reader.times

    with sqlite3.Connection(dbname) as connection:
        reader.read(connection)
    assert reader.times[timestamp].times.tolist() == data_array

def test_read_multiple_arrays():
    filename, data_arrays = "stability.txt", [[3.0, 5.0, 1.1, 12.3], [4.0, 7.1, 3.2]]
    timestamp1 = build_timestamp_str()
    time.sleep(0.02)
    timestamp2 = build_timestamp_str()
    timestamps = [timestamp1, timestamp2]
    assert timestamps[0] != timestamps[1]
    reader = StabilityReader(filename)

    initial_iter = True
    for data_array, timestamp in zip(data_arrays, timestamps):
        with open(filename, 'w') as file:
            for data in data_array:
                file.write(str(data) + '\n')
        reader.cache(metadata, timestamp)
        print(reader.times)
        assert reader.times[timestamp].times.tolist() == data_array

        with sqlite3.Connection(dbname) as connection:
            if initial_iter:
                reader.write(connection, True, True)
                initial_iter = False
            else:
                reader.write(connection, False, True)
        assert not reader.times

    
    with sqlite3.Connection(dbname) as connection:
        reader.read(connection)

    print(reader.times)
    assert len(reader.times.values()) == len(data_arrays)
    for data, expected in zip(reader.times.values(), data_arrays):
        assert data.times.tolist() == expected