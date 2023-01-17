#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "../quantum_verifier.h"

namespace py = pybind11;

PYBIND11_MODULE(pyqcmc, m) {
    m.doc() = "python wrapper for Quantum Circuit Model Checker"; // Optional module docstring

    py::class_<QuantumVerifier>(m, "QuantumVerifier")
        .def(py::init<>())
        .def(py::init<unsigned int>())
        .def("setNumQubits", &QuantumVerifier::setNumQubits, "setNumQubits")
        .def("i", &QuantumVerifier::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &QuantumVerifier::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &QuantumVerifier::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &QuantumVerifier::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &QuantumVerifier::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &QuantumVerifier::GetProbability, "GetProbability")
        .def("y", &QuantumVerifier::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &QuantumVerifier::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &QuantumVerifier::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &QuantumVerifier::ApplySGate, "ApplySGate")
        .def("t", &QuantumVerifier::ApplyTGate, "ApplyTGate") 
        .def("cz", &QuantumVerifier::ApplyCZGate, "ApplyCZGate")
        .def("cx", &QuantumVerifier::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &QuantumVerifier::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("gp", &QuantumVerifier::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &QuantumVerifier::ApplyCPGate, "ApplyCPGate")
        .def("cs", &QuantumVerifier::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &QuantumVerifier::ApplyCSwapGate, "ApplyCSwapGate");

}
