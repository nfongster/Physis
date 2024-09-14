#pragma once
#include <pybind11/pybind11.h>
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
}
