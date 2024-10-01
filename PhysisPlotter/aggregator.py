from typing import Dict
from functools import reduce
from datetime import datetime
import os, re, sqlite3
import numpy as np
from utility import *


def build_timestamp_str() -> str:
    """Returns the current date-time with format YYYYMMDDHHMMSSMMM."""
    timestamp = datetime.now()
    return timestamp.strftime('%Y%m%d%H%M%S') + f"{timestamp.microsecond // 1000:03d}"


class StabilityReader:
    def __init__(self, path: str):
        self.path = path
        self.times = {}  # metadata -> times

    def cache(self, metadata: SimulationMetadata) -> None:
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"File not found: {self.path}")
        
        self.times, render_times = {}, []
        with open(self.path, 'r') as file:
            for line in file:
                render_times.append(float(line))
        
        self.times[metadata] = np.array(render_times)

    def write(self, connection: sqlite3.Connection, timestamp_str: str, overwrite: bool, dump_data: bool) -> None:
        cursor = connection.cursor()
        if overwrite: cursor.execute("DROP TABLE IF EXISTS stability")

        cursor.execute(("CREATE TABLE IF NOT EXISTS stability (datetime TEXT, "
                "dt FLOAT, "
                "pcount INTEGER, "
                "scalar FLOAT,"
                "rendertime FLOAT," 
                "totaltime FLOAT," 
                "data BLOB," 
                "datalen INT)"))
        
        for m, data in self.times.items():
            datablob = data.tobytes()
            dt = m.dt.microseconds / 1000
            t_render = m.t_render.microseconds / 1000
            t_total = m.t_total.microseconds / 1000
            pcount = m.pcount
            scalar = m.scalar
            cursor.execute("INSERT INTO stability VALUES (?, ?, ?, ?, ?, ?, ?, ?)", 
                (timestamp_str, dt, pcount, scalar, t_render, t_total, datablob, len(data)))
            connection.commit()
        
        if dump_data: self.times = {}

    def read(self, connection: sqlite3.Connection) -> None:
        cursor = connection.cursor()
        rows = cursor.execute("SELECT datetime, dt, pcount, scalar, rendertime, totaltime, data, datalen FROM stability").fetchall()
        for row in rows:
            dt = row[1]
            pcount = row[2]
            scalar = row[3]
            render_time = row[4]
            t_total = row[5]
            metadata = SimulationMetadata(scalar, 
                                          timedelta(seconds=dt),
                                          timedelta(seconds=render_time), 
                                          timedelta(seconds=t_total),
                                          pcount)
            # hack
            data = np.zeros(row[7])
            self.times[metadata] = np.frombuffer(row[6], dtype=data.dtype)


class TrajectoryReader:
    def __init__(self, path: str):
        self.path = path
        self.trajectory = {}  # time_id -> kinematic param

    def cache(self, metadata: SimulationMetadata) -> None:  # TODO: Get rid of metadata, only for stability
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"File not found: {self.path}")
        
        self.trajectory = {}
        with open(self.path, 'r') as file:
            for i, line in enumerate(file):
                data = line.split('\t')
                pid = int(data[0])
                time = timedelta(seconds=float(data[1]))
                r_text = re.sub(r'[( )]', '', data[2]).split(',')
                rx, ry = float(r_text[0]), float(r_text[1])
                v_text = re.sub(r'[( )]', '', data[3]).split(',')
                vx, vy = float(v_text[0]), float(v_text[1])
                a_text = re.sub(r'[( )]', '', data[4]).split(',')
                ax, ay = float(a_text[0]), float(a_text[1])
                self.trajectory[i] = KinematicData(pid, time, rx, ry, vx, vy, ax, ay)

    def write(self, connection: sqlite3.Connection, timestamp_str: str, overwrite: bool, dump_data: bool) -> None:
        particleids = set([data.pid for data in self.trajectory.values()])
        pcount = len(particleids)
        totaltime_sec = reduce(lambda t1, t2: t1 + t2, [data.time.total_seconds() for data in self.trajectory.values()])
        cursor = connection.cursor()
        if overwrite:
            cursor.execute("DROP TABLE IF EXISTS Trajectories")
            cursor.execute("DROP TABLE IF EXISTS Particles")
            cursor.execute("DROP TABLE IF EXISTS ParticleStates")

        cursor.execute(
            ("CREATE TABLE IF NOT EXISTS Trajectories ("
                "RUNID	    INTEGER,"
                "TIMESTAMP	TEXT,"
                "PCOUNT	    INTEGER,"
                "RUNTIME	FLOAT,"
                "PRIMARY KEY(RUNID AUTOINCREMENT)"
            ")"))
        
        cursor.execute("INSERT INTO Trajectories (TIMESTAMP, PCOUNT, RUNTIME) VALUES (?, ?, ?)",
                           (timestamp_str, pcount, totaltime_sec))
        runid = cursor.execute("SELECT MAX(RUNID) FROM Trajectories").fetchall()[0][0]
        
        cursor.execute(
            ("CREATE TABLE IF NOT EXISTS Particles ("
                "PID	    INTEGER,"
                "RUNID	    INTEGER,"
                "PRIMARY KEY(PID AUTOINCREMENT),"
                "FOREIGN KEY(RUNID) REFERENCES Trajectories(RUNID) ON DELETE CASCADE"
            ")"))

        max_pid = cursor.execute("SELECT MAX(PID) FROM Particles").fetchall()[0][0]
        for pid in particleids:
            if max_pid != None:
                pid += max_pid + 1
            cursor.execute("INSERT INTO Particles (PID, RUNID) VALUES (?, ?)", (pid, runid))
        
        cursor.execute(
            ("CREATE TABLE IF NOT EXISTS ParticleStates ("
                "STATEID	INTEGER,"
                "PID	    INTEGER,"
                "SIMTIME	FLOAT,"
                "RX	        FLOAT,"
                "RY	        FLOAT,"
                "VX	        FLOAT,"
                "VY	        FLOAT,"
                "AX	        FLOAT,"
                "AY	        FLOAT,"
                "PRIMARY KEY(STATEID AUTOINCREMENT),"
                "FOREIGN KEY(PID) REFERENCES Particles(PID) ON DELETE CASCADE"
            ")"))
        
        for _, data in self.trajectory.items():
            cursor.execute("INSERT INTO ParticleStates (PID, SIMTIME, RX, RY, VX, VY, AX, AY) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                           (data.pid, data.time.total_seconds(), data.rx, data.ry, data.vx, data.vy, data.ax, data.ay))
            connection.commit()

        if dump_data: self.trajectory = {}

    def read(self, connection: sqlite3.Connection) -> None:
        cursor = connection.cursor()
        runid = int(cursor.execute("SELECT MAX(RUNID) FROM Trajectories").fetchall()[0][0])
        particle_states = cursor.execute("SELECT * FROM ParticleStates ps JOIN Particles p ON ps.pid = p.pid JOIN Trajectories t ON p.runid = t.runid WHERE t.runid = (?)", (runid,)).fetchall()
        for i, row in enumerate(particle_states):
            pid = row[1]
            time = row[2]
            r = (row[3], row[4])
            v = (row[5], row[6])
            a = (row[7], row[8])
            self.trajectory[i] = (pid, time, r, v, a)


class DataAggregator:
    def __init__(self, filepaths: Dict[DataType, str]):
        if filepaths is None:
            raise ValueError("Supplied datafiles must be a non-empty dict!")
        
        self.readers = {}
        for type, path in filepaths.items():
            self.readers[type] = self._create_reader(type, path)

    def _create_reader(self, type: DataType, path: str):  # TODO: Create interface
        if type == DataType.STABILITY:  return StabilityReader(path)
        if type == DataType.TRAJECTORY: return TrajectoryReader(path)

    def cache(self, metadata: SimulationMetadata) -> None:
        for reader in self.readers.values():
            reader.cache(metadata)  # TODO: Only needed for stability reader

    def write(self, dbfilename: str, overwrite: bool, dump_data: bool) -> None:  # todo: replace with write_all, add write(DataType)
        timestamp_str = build_timestamp_str()
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.write(connection, timestamp_str, overwrite, dump_data)
        
        connection.close()

    def read(self, dbfilename: str) -> None:
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.read(connection)
        
        connection.close()