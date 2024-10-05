from enum import Enum
from typing import Dict
from dataclasses import dataclass
from datetime import datetime, timedelta
import numpy as np


@dataclass(frozen=True)  # frozen because dict keys must be immutable
class KinematicData:
    pid:  int        # Particle ID.
    time: timedelta  # Time relative to start of simulation.
    rx:   float      # X position.
    ry:   float      # Y position.
    vx:   float      # X velocity.
    vy:   float      # Y velocity.
    ax:   float      # X acceleration.
    ay:   float      # Y acceleration.


@dataclass(frozen=True)
class SimulationMetadata:
    scalar:   float      # Determines simulation speed.
    dt:       timedelta  # Simulation integration time step size.
    t_render: timedelta  # Time to simulate rendering.
    t_total:  timedelta  # Total time to run the simulation.
    pcount:   int        # Number of particles in the simulation.


@dataclass(frozen=True)
class StabilityData:
    metadata: SimulationMetadata
    times:    np.array


@dataclass(frozen=True)
class TrajectoryData:
    metadata:       SimulationMetadata
    trajectories:   Dict[int, KinematicData]


class DataType(Enum):
    STABILITY = 0
    TRAJECTORY = 1


def build_timestamp_str() -> str:
    """Returns the current date-time with format YYYYMMDDHHMMSSMMM."""
    timestamp = datetime.now()
    return timestamp.strftime('%Y%m%d%H%M%S') + f"{timestamp.microsecond // 1000:03d}"