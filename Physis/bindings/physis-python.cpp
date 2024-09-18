#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // required for shared pointerse
#include "..\Physis.h"

#include <string>

namespace py = pybind11;

PYBIND11_MODULE(physis, m) {
    m.doc() = "Python module for physis";  // module docstring

    py::class_<Vec1>(m, "Vec1")
        .def(py::init())
        .def(py::init<double>(), "Vector position")
        .def("equals", &Vec1::Equals, "assess vector equality",
            py::arg("other"),
            py::arg("tolerance") = DEFAULT_TOLERANCE)
        .def("__repr__",
            [](const Vec1& v)
            {
                return "(" + std::to_string(v.X) + ")";
            });

    py::class_<Vec2>(m, "Vec2")
        .def(py::init())
        .def(py::init<double, double>(), "Vector position")
        .def("equals", &Vec2::Equals, "assess vector equality",
            py::arg("other"),
            py::arg("tolerance") = DEFAULT_TOLERANCE)
        .def("__repr__",
            [](const Vec2& v)
            {
                return "(" + std::to_string(v.X) + ", " + std::to_string(v.Y) + ")";
            });

    py::class_<Vec3>(m, "Vec3")
        .def(py::init())
        .def(py::init<double, double, double>(), "Vector position")
        .def("equals", &Vec3::Equals, "assess vector equality",
            py::arg("other"),
            py::arg("tolerance") = DEFAULT_TOLERANCE)
        .def("__repr__",
            [](const Vec3& v)
            {
                return "(" + std::to_string(v.X) + ", " + std::to_string(v.Y) + ", " + std::to_string(v.Z) + ")";
            });

    py::class_<InitialConditions>(m, "InitialConditions")
        .def(py::init())
        .def(py::init<Vec2, Vec2, Vec2>())
        .def_readwrite("r", &InitialConditions::r)
        .def_readwrite("v", &InitialConditions::v)
        .def_readwrite("a", &InitialConditions::a);

    py::class_<TimeConfig>(m, "TimeConfig")
        .def(py::init())
        .def(py::init<const double&, const double&, const double&>());

    // You must designate Particle as a shared pointer.
    // This provides consistency when accessing the particle map in ParticleSystem.
    py::class_<Particle, std::shared_ptr<Particle>>(m, "Particle")
        .def(py::init())
        .def(py::init<InitialConditions>())
        .def("position", &Particle::GetPosition, "returns the particle's position")
        .def("velocity", &Particle::GetVelocity, "returns the particle's velocity")
        .def("acceleration", &Particle::GetAcceleration, "returns the particle's acceleration")
        .def("step", &Particle::Step, "iterates the particle's state",
            py::arg("dt"));

    py::class_<ParticleSystem, std::shared_ptr<ParticleSystem>>(m, "ParticleSystem")
        .def(py::init())
        .def("particles", &ParticleSystem::GetParticles, "gets all particles in the system")
        .def("add", &ParticleSystem::Add, "add a new particle to the system",
            py::arg("ic"))
        .def("step", &ParticleSystem::Step, "steps all particles in the system")
        .def("update", &ParticleSystem::Update, "updates the particle at the specified location",
            py::arg("index"),
            py::arg("ic"));

    py::class_<SystemState, std::shared_ptr<SystemState>>(m, "SystemState")
        .def(py::init())
        .def("add", &SystemState::AddParticle, "add a new particle to the system",
            py::arg("ic"))
        .def("current", &SystemState::GetCurrent, "gets the current state of the system")
        .def("previous", &SystemState::GetPrevious, "gets the previous state of the system");
}
