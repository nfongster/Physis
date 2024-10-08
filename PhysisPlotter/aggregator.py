from typing import Dict
from functools import reduce
import os, re, sqlite3
import numpy as np
from utility import *


class ReaderInterface():
    def __init__(self, path: str):
        """
        Initializes a new Reader that consumes data and reads/writes to the database.

        Args:
            path (str): Expected (relative) filepath of the text file emitted by the Physis engine.
        """
        pass

    def cache(self, metadata: SimulationMetadata, timestamp: str) -> None:
        """
        Caches data emitted by the Physis engine into the Reader.

        Args:
            metadata (SimulationMetadata): Metadata about the simulation run.
            timestamp (str): Desired timestamp with which to mark the simulation run.
        """
        pass

    def write(self, connection: sqlite3.Connection, reset_cache: bool) -> None:
        """
        Instructs the Reader to write its cached data to the database.

        Args:
            connection (Connection): Connection to the database.
            overwrite_db (bool): Whether to overwrite all data in the database.
            reset_cache (bool): Whether to reset the Reader's cache after it has written to the database.
        """
        pass

    def read(self, connection: sqlite3.Connection) -> None:
        """
        Instructs the Reader to read and cache stability data from the database.

        Args:
            connection (Connection): Connection to the database.
        """
        pass

    def delete(self, cursor: sqlite3.Cursor) -> None:
        """
        Instructs the Reader to drop the table(s) for which it is responsible from the database, if it/they exist.

        Args:
            cursor (Cursor): Connection cursor to the database.
        """
        pass


class StabilityReader(ReaderInterface):
    def __init__(self, path: str):
        self.path:  str = path
        self.times: Dict[str, StabilityData] = {}


    def cache(self, metadata: SimulationMetadata, timestamp: str) -> None:
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"File not found: {self.path}")
        
        render_times = []
        with open(self.path, 'r') as file:
            for line in file:
                render_times.append(float(line))
        
        self.times[timestamp] = StabilityData(metadata, np.array(render_times))

    
    def write(self, connection: sqlite3.Connection, reset_cache: bool) -> None:
        cursor = connection.cursor()
        cursor.execute(("CREATE TABLE IF NOT EXISTS stability (datetime TEXT, "
                "dt_sec FLOAT, "
                "pcount INTEGER, "
                "scalar FLOAT,"
                "rendertime_sec FLOAT," 
                "totaltime_sec FLOAT," 
                "data BLOB," 
                "datalen INT)"))
        
        for timestamp, simdata in self.times.items():
            m = simdata.metadata
            datablob = simdata.times.tobytes()
            dt = m.dt.total_seconds()
            t_render = m.t_render.total_seconds()
            t_total = m.t_total.total_seconds()
            pcount = m.pcount
            scalar = m.scalar
            cursor.execute("INSERT INTO stability VALUES (?, ?, ?, ?, ?, ?, ?, ?)", 
                (timestamp, dt, pcount, scalar, t_render, t_total, datablob, len(simdata.times)))
            connection.commit()
        
        if reset_cache: self.times = {}


    def read(self, connection: sqlite3.Connection) -> None:
        cursor = connection.cursor()
        rows = cursor.execute("SELECT * FROM stability").fetchall()
        for row in rows:
            timestamp = row[0]
            dt = row[1]
            pcount = row[2]
            scalar = row[3]
            render_time = row[4]
            t_total = row[5]
            metadata = SimulationMetadata(scalar, 
                                          timedelta(microseconds=dt),
                                          timedelta(microseconds=render_time), 
                                          timedelta(microseconds=t_total),
                                          pcount)
            # hack
            data = np.zeros(row[7])
            self.times[timestamp] = StabilityData(metadata, np.frombuffer(row[6], dtype=data.dtype))


    def delete(self, cursor: sqlite3.Cursor) -> None:
        cursor.execute("DROP TABLE IF EXISTS stability")


class TrajectoryReader(ReaderInterface):
    def __init__(self, path: str):
        self.path:       str = path
        self.trajectory: Dict[str, TrajectoryData] = {}


    def cache(self, metadata: SimulationMetadata, timestamp: str) -> None:
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"File not found: {self.path}")
        
        data_dict: Dict[int, KinematicData] = {}
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
                data_dict[i] = KinematicData(pid, time, rx, ry, vx, vy, ax, ay)
        self.trajectory[timestamp] = TrajectoryData(metadata, data_dict)


    def write(self, connection: sqlite3.Connection, reset_cache: bool) -> None:
        cursor = connection.cursor()
        cursor.execute(
            ("CREATE TABLE IF NOT EXISTS Trajectories ("
                "RUNID	    INTEGER,"
                "TIMESTAMP	TEXT,"
                "DT_SEC     FLOAT,"
                "PCOUNT	    INTEGER,"
                "SCALAR     FLOAT,"
                "RENDERTIME FLOAT,"
                "RUNTIME	FLOAT,"
                "PRIMARY KEY(RUNID AUTOINCREMENT)"
            ")"))
        
        for timestamp, trajdata in self.trajectory.items():
            m, data_dict = trajdata.metadata, trajdata.trajectories
            particleids = set([data.pid for data in data_dict.values()])
            # Only accumulate times from the first particle.  Otherwise, you will get: t_total_actual * pcount
            totaltime_sec = reduce(lambda t1, t2: t1 + t2, [data.time.total_seconds() for data in data_dict.values() if data.pid == 0])
            # if len(particleids) != m.pcount or totaltime_sec != m.t_total.seconds:
            #     raise Exception(f"Metadata did not match actual data!"
            #                     f" pcount: metadata was {m.pcount}, actual was {len(particleids)}."
            #                     f" t_total: metadata was {m.t_total.seconds}, actual was {totaltime_sec}.")
            if len(particleids) != m.pcount:
                raise Exception(f"Metadata did not match actual data!"
                                f" pcount: metadata was {m.pcount}, actual was {len(particleids)}.")
            
            cursor.execute("INSERT INTO Trajectories (TIMESTAMP, DT_SEC, PCOUNT, SCALAR, RENDERTIME, RUNTIME) VALUES (?, ?, ?, ?, ?, ?)",
                            (timestamp, m.dt.total_seconds(), m.pcount, m.scalar, m.t_render.total_seconds(), m.t_total.total_seconds()))
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
            
            if max_pid == None: max_pid = -1
            for data in data_dict.values():
                cursor.execute("INSERT INTO ParticleStates (PID, SIMTIME, RX, RY, VX, VY, AX, AY) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                            (data.pid + max_pid + 1, data.time.total_seconds(), data.rx, data.ry, data.vx, data.vy, data.ax, data.ay))
                connection.commit()
        if reset_cache: self.trajectory = {}


    def read(self, connection: sqlite3.Connection) -> None:
        cursor = connection.cursor()
        trajectories = cursor.execute("SELECT * FROM Trajectories").fetchall()
        for traj in trajectories:
            runid, timestamp, dt, pcount, scalar, rendertime, runtime = traj[0], traj[1], traj[2], traj[3], traj[4], traj[5], traj[6]
            metadata = SimulationMetadata(scalar, 
                                          timedelta(seconds=dt),
                                          timedelta(seconds=rendertime), 
                                          timedelta(seconds=runtime),
                                          pcount)

            particle_states = cursor.execute("SELECT * FROM ParticleStates ps JOIN Particles p ON ps.pid = p.pid JOIN Trajectories t ON p.runid = t.runid WHERE t.runid = (?)", (runid,)).fetchall()
            data_dict: Dict[int, KinematicData] = {}
            for i, row in enumerate(particle_states):
                pid = row[1]
                time = timedelta(seconds=row[2])
                rx, ry = row[3], row[4]
                vx, vy = row[5], row[6]
                ax, ay = row[7], row[8]
                data_dict[i] = KinematicData(pid, time, rx, ry, vx, vy, ax, ay)
            self.trajectory[timestamp] = TrajectoryData(metadata, data_dict)

    
    def delete(self, cursor: sqlite3.Cursor) -> None:
        cursor.execute("DROP TABLE IF EXISTS Trajectories")
        cursor.execute("DROP TABLE IF EXISTS Particles")
        cursor.execute("DROP TABLE IF EXISTS ParticleStates")


class DataAggregator:
    def __init__(self, filepaths: Dict[DataType, str]):
        """
        Initializes the aggregator with the appropriate readers.

        Args:
            filepaths (Dict[DataType, str]): Filepath for each data file emitted by the Physis engine.
        """
        if filepaths is None:
            raise ValueError("Supplied datafiles must be a non-empty dict!")
        
        self.readers: Dict[DataType, ReaderInterface] = {}
        for type, path in filepaths.items():
            self.readers[type] = self._create_reader(type, path)


    def _create_reader(self, type: DataType, path: str):
        if type == DataType.STABILITY:      return StabilityReader(path)
        if type == DataType.TRAJECTORY:     return TrajectoryReader(path)


    # TODO: replace with cache_all, add cache(DataType)
    def cache(self, metadata: SimulationMetadata) -> None:
        timestamp_str = build_timestamp_str()
        for reader in self.readers.values():
            reader.cache(metadata, timestamp_str)

    # TODO: replace with write_all, add write(DataType)
    def write(self, dbfilename: str, reset_cache: bool) -> None:
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.write(connection, reset_cache)
        
        connection.close()

    # TODO: Replace with read_all, add read(DataType)
    def read(self, dbfilename: str) -> None:
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.read(connection)
        
        connection.close()

    
    def delete(self, dbfilename: str) -> None:
        connection = sqlite3.connect(dbfilename)
        cursor = connection.cursor()

        for reader in self.readers.values():
            reader.delete(cursor)
        
        connection.close()