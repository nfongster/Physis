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
}
