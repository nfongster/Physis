from typing import Dict
from functools import reduce
import os, re, sqlite3
import numpy as np
from utility import *


class ReaderInterface():
    def cache(self, metadata: SimulationMetadata, timestamp: str) -> None:
        pass

    def write(self, connection: sqlite3.Connection, overwrite_db: bool, reset_cache: bool) -> None:
        pass

    def read(self, connection: sqlite3.Connection) -> None:
        pass


class StabilityReader(ReaderInterface):
    def __init__(self, path: str):
        """
        Initializes a new StabilityReader that consumes stability data and reads/writes to the database.

        Args:
            path (str): Expected (relative) filepath of the stability text file emitted by the Physis engine.
        """
        self.path: str = path
        self.times: Dict[str, StabilityData] = {}


    def cache(self, metadata: SimulationMetadata, timestamp: str) -> None:
        """
        Caches data emitted by the Physis engine into the StabilityReader.

        Args:
            metadata (SimulationMetadata): Metadata about the simulation run.
            timestamp (str): Desired timestamp with which to mark the simulation run.
        """
        if not os.path.exists(self.path):
            raise FileNotFoundError(f"File not found: {self.path}")
        
        render_times = []
        with open(self.path, 'r') as file:
            for line in file:
                render_times.append(float(line))
        
        self.times[timestamp] = StabilityData(metadata, np.array(render_times))

    
    def write(self, connection: sqlite3.Connection, overwrite_db: bool, reset_cache: bool) -> None:
        """
        Instructs the StabilityReader to write its cached data to the database.

        Args:
            connection (Connection): Connection to the database.
            overwrite_db (bool): Whether to overwrite all stability data in the database.
            reset_cache (bool): Whether to reset the StabilityReader's cache after it has written to the database.
        """
        cursor = connection.cursor()
        if overwrite_db: cursor.execute("DROP TABLE IF EXISTS stability")

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
            dt = m.dt.microseconds / 1000
            t_render = m.t_render.microseconds / 1000
            t_total = m.t_total.microseconds / 1000
            pcount = m.pcount
            scalar = m.scalar
            cursor.execute("INSERT INTO stability VALUES (?, ?, ?, ?, ?, ?, ?, ?)", 
                (timestamp, dt, pcount, scalar, t_render, t_total, datablob, len(simdata.times)))
            connection.commit()
        
        if reset_cache: self.times = {}


    def read(self, connection: sqlite3.Connection) -> None:
        """
        Instructs the StabilityReader to read and cache stability data from the database.

        Args:
            connection (Connection): Connection to the database.
        """
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
                                          timedelta(seconds=dt),
                                          timedelta(seconds=render_time), 
                                          timedelta(seconds=t_total),
                                          pcount)
            # hack
            data = np.zeros(row[7])
            self.times[timestamp] = StabilityData(metadata, np.frombuffer(row[6], dtype=data.dtype))


class TrajectoryReader(ReaderInterface):
    def __init__(self, path: str):
        """
        Initializes a new TrajectoryReader that consumes trajectory data and reads/writes to the database.

        Args:
            path (str): Expected (relative) filepath of the trajectory text file emitted by the Physis engine.
        """
        self.path: str = path
        self.trajectory: Dict[str, TrajectoryData] = {}


    def cache(self, metadata: SimulationMetadata, timestamp: str) -> None:
        """
        Caches data emitted by the Physis engine into the TrajectoryReader.

        Args:
            timestamp (str): Desired timestamp with which to mark the simulation run.
        """
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


    def write(self, connection: sqlite3.Connection, overwrite_db: bool, reset_cache: bool) -> None:
        cursor = connection.cursor()
        if overwrite_db:
            cursor.execute("DROP TABLE IF EXISTS Trajectories")
            cursor.execute("DROP TABLE IF EXISTS Particles")
            cursor.execute("DROP TABLE IF EXISTS ParticleStates")

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
            if len(particleids) != m.pcount or totaltime_sec != m.t_total.seconds:
                raise Exception(f"Metadata did not match actual data!"
                                f" pcount: metadata was {m.pcount}, actual was {len(particleids)}."
                                f" t_total: metadata was {m.t_total.seconds}, actual was {totaltime_sec}.")
            
            cursor.execute("INSERT INTO Trajectories (TIMESTAMP, DT_SEC, PCOUNT, SCALAR, RENDERTIME, RUNTIME) VALUES (?, ?, ?, ?, ?, ?)",
                            (timestamp, m.dt.seconds, m.pcount, m.scalar, m.t_render.seconds, m.t_total.seconds))
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
            
            for _, data in data_dict.items():
                cursor.execute("INSERT INTO ParticleStates (PID, SIMTIME, RX, RY, VX, VY, AX, AY) VALUES (?, ?, ?, ?, ?, ?, ?, ?)",
                            (data.pid, data.time.total_seconds(), data.rx, data.ry, data.vx, data.vy, data.ax, data.ay))
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


    def cache(self, metadata: SimulationMetadata) -> None:
        for reader in self.readers.values():
            reader.cache(metadata)


    def write(self, dbfilename: str, overwrite_db: bool, reset_cache: bool) -> None:  # todo: replace with write_all, add write(DataType)
        timestamp_str = build_timestamp_str()
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.write(connection, timestamp_str, overwrite_db, reset_cache)
        
        connection.close()


    def read(self, dbfilename: str) -> None:
        connection = sqlite3.connect(dbfilename)

        for reader in self.readers.values():
            reader.read(connection)
        
        connection.close()