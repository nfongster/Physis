#pragma once
#include <pybind11/pybind11.h>
#include <pybind11/stl.h>  // required for shared pointers
#include <pybind11/chrono.h>  // required for chrono::duration
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

    py::class_<KinematicParameters>(m, "KinematicParameters")
        .def(py::init())
        .def(py::init<Vec2, Vec2, Vec2>())
        .def_readwrite("r", &KinematicParameters::r)
        .def_readwrite("v", &KinematicParameters::v)
        .def_readwrite("a", &KinematicParameters::a);

    py::class_<TimeConfig>(m, "TimeConfig")
        .def(py::init())
        .def(py::init<const std::chrono::duration<double>&, const std::chrono::duration<double>&, const double&>());

    // You must designate Particle as a shared pointer.
    // This provides consistency when accessing the particle map in ParticleSystem.
    py::class_<Particle, std::shared_ptr<Particle>>(m, "Particle")
        .def(py::init())
        .def(py::init<KinematicParameters, float>())
        .def("position", &Particle::GetPosition, "returns the particle's position")
        .def("velocity", &Particle::GetVelocity, "returns the particle's velocity")
        .def("acceleration", &Particle::GetAcceleration, "returns the particle's acceleration")
        .def("step", &Particle::Step, "iterates the particle's state",
            py::arg("dt"));

    py::class_<ParticleSystem, std::shared_ptr<ParticleSystem>>(m, "ParticleSystem")
        .def(py::init())
        .def("particles", &ParticleSystem::GetParticles, "gets all particles in the system")
        .def("add", &ParticleSystem::Add, "add a new particle to the system",
            py::arg("kp"),
            py::arg("radius"))
        .def("step", &ParticleSystem::Step, "steps all particles in the system")
        .def("update", &ParticleSystem::Update, "updates the particle at the specified location",
            py::arg("index"),
            py::arg("ic"));

    py::class_<SystemState, std::shared_ptr<SystemState>>(m, "SystemState")
        .def(py::init())
        .def("add", &SystemState::AddParticle, "add a new particle to the system",
            py::arg("kp"),
            py::arg("radius"))
        .def("current", &SystemState::GetCurrent, "gets the current state of the system")
        .def("previous", &SystemState::GetPrevious, "gets the previous state of the system");

    py::class_<SampleEngine>(m, "SampleEngine")
        .def(py::init<const TimeConfig&>())
        .def("add", py::overload_cast<>(&SampleEngine::AddParticle), "add a new particle to the engine")
        .def("add", py::overload_cast<const KinematicParameters&>(&SampleEngine::AddParticle), "add a new particle to the engine")
        .def("run", &EngineBase::Run, "run the engine")
        .def("sample", &SampleEngine::Sample, "get the current state of the system");

    py::class_<BenchmarkEngine>(m, "BenchmarkEngine")
        .def(py::init<const TimeConfig&, const std::string&, const std::chrono::duration<double, std::milli>&>())
        .def("add", py::overload_cast<>(&BenchmarkEngine::AddParticle), "add a new particle to the engine")
        .def("add", py::overload_cast<const KinematicParameters&>(&BenchmarkEngine::AddParticle), "add a new particle to the engine")
        .def("run", &EngineBase::Run, "run the engine");
}
