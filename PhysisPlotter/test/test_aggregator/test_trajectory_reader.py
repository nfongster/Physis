import pytest, time
from aggregator import *

metadata = SimulationMetadata(1, timedelta(seconds=0), timedelta(seconds=0), timedelta(seconds=0), 1)
filename, dbname = "trajectory.txt", "results.db"
default_data = KinematicData(0, timedelta(seconds=0), 0.0, 0.0, 0.0, 0.0, 0.0, 0.0)


def test_constructor():
    reader = TrajectoryReader(filename)
    assert reader


def test_cache_incomplete_filename():
    with pytest.raises(FileNotFoundError):
        reader = TrajectoryReader("badfilename.txt")
        reader.cache(metadata, build_timestamp_str())


def test_cache_valid_filename():
    timestamp = build_timestamp_str()
    reader = TrajectoryReader(filename)

    with open(filename, 'w') as file:
        file.write(f"{default_data.pid}\t")
        file.write(f"{default_data.time.seconds}\t")
        file.write(f"({default_data.rx}, {default_data.ry})\t")
        file.write(f"({default_data.vx}, {default_data.vy})\t")
        file.write(f"({default_data.ax}, {default_data.ay})\t")
    reader.cache(metadata, timestamp)


def test_write():
    timestamp = build_timestamp_str()
    reader = TrajectoryReader(filename)

    with open(filename, 'w') as file:
        file.write(f"{default_data.pid}\t")
        file.write(f"{default_data.time.seconds}\t")
        file.write(f"({default_data.rx}, {default_data.ry})\t")
        file.write(f"({default_data.vx}, {default_data.vy})\t")
        file.write(f"({default_data.ax}, {default_data.ay})\t")
    reader.cache(metadata, timestamp)

    with sqlite3.Connection(dbname) as connection:
        reader.write(connection, True, True)


def test_read():
    timestamp = build_timestamp_str()
    reader = TrajectoryReader(filename)

    with open(filename, 'w') as file:
        file.write(f"{default_data.pid}\t")
        file.write(f"{default_data.time.seconds}\t")
        file.write(f"({default_data.rx}, {default_data.ry})\t")
        file.write(f"({default_data.vx}, {default_data.vy})\t")
        file.write(f"({default_data.ax}, {default_data.ay})\t")
    reader.cache(metadata, timestamp)
    assert reader.trajectory[timestamp].trajectories[0] == default_data

    with sqlite3.Connection(dbname) as connection:
        reader.write(connection, True, True)
    assert not reader.trajectory

    with sqlite3.Connection(dbname) as connection:
        reader.read(connection)
    assert reader.trajectory[timestamp].trajectories[0] == default_data


def test_read_multiple_arrays():
    metadata_2particles = SimulationMetadata(1, timedelta(seconds=1.0), timedelta(seconds=0), timedelta(seconds=1.0), 2)

    data_run1 = [
        KinematicData(0, timedelta(seconds=0.0), 0.0, 0.0, 1.0, 0.0, 0.0, 0.0),
        KinematicData(0, timedelta(seconds=1.0), 1.0, 0.0, 1.0, 0.0, 0.0, 0.0),
        KinematicData(1, timedelta(seconds=0.0), 0.0, 0.0, 2.0, 0.0, 0.0, 0.0),
        KinematicData(1, timedelta(seconds=1.0), 2.0, 0.0, 2.0, 0.0, 0.0, 0.0)]
    
    data_run2 = [
        KinematicData(0, timedelta(seconds=0.0), 0.0, 0.0, 1.5, 0.0, 0.0, 0.0),
        KinematicData(0, timedelta(seconds=1.0), 1.5, 0.0, 1.5, 0.0, 0.0, 0.0),
        KinematicData(1, timedelta(seconds=0.0), 0.0, 0.0, 2.5, 0.0, 0.0, 0.0),
        KinematicData(1, timedelta(seconds=1.0), 2.5, 0.0, 2.5, 0.0, 0.0, 0.0)]
    timestamp1 = build_timestamp_str()
    time.sleep(0.02)
    timestamp2 = build_timestamp_str()
    timestamps = [timestamp1, timestamp2]
    assert timestamps[0] != timestamps[1]
    reader = TrajectoryReader(filename)

    initial_iter = True
    for data_arrays, timestamp in zip([data_run1, data_run2], timestamps):
        with open(filename, 'w') as file:
            for data in data_arrays:
                file.write(f"{data.pid}\t")
                file.write(f"{data.time.seconds}\t")
                file.write(f"({data.rx}, {data.ry})\t")
                file.write(f"({data.vx}, {data.vy})\t")
                file.write(f"({data.ax}, {data.ay})\n")
        
        reader.cache(metadata_2particles, timestamp)
        for i, data in enumerate(data_arrays):
            assert reader.trajectory[timestamp].trajectories[i] == data

    with sqlite3.Connection(dbname) as connection:
        if initial_iter:
            reader.write(connection, True, True)
            initial_iter = False
        else:
            reader.write(connection, False, True)
    assert not reader.trajectory

    with sqlite3.Connection(dbname) as connection:
        reader.read(connection)

    for expected, actual in zip([timestamp1, timestamp2], reader.trajectory.keys()):
        assert expected == actual
    
    for expected_data_arr, actual_data_arr in zip([data_run1, data_run2], reader.trajectory.values()):
        for expected_data, actual_data in zip(expected_data_arr, actual_data_arr.trajectories.values()):
            assert expected_data == actual_data