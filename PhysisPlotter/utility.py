from enum import Enum
from dataclasses import dataclass
from datetime import timedelta


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


class DataType(Enum):
    STABILITY = 0
    TRAJECTORY = 1