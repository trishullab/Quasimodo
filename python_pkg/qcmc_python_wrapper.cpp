#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "../quantum_verifier.h"

namespace py = pybind11;

PYBIND11_MODULE(pyqcmc, m) {
    m.doc() = "python wrapper for Quantum Circuit Model Checker"; // Optional module docstring

    // py::class_<QuantumVerifier>(m, "QuantumVerifier");
        // .def(py::init<>());
        // .def(py::init<unsigned int>())
        // .def("setNumQubits", &QuantumVerifier::setNumQubits, "setNumQubits");

}
