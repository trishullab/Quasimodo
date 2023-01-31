#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "../quantum_verifier.h"

namespace py = pybind11;

PYBIND11_MODULE(pyqcmc, m) {
    m.doc() = "python wrapper for Quantum Circuit Model Checker"; // Optional module docstring

    py::class_<QuantumVerifier>(m, "QuantumVerifier");
        // .def(py::init<>())
        // .def(py::init<unsigned int>());
    
    py::class_<CFLOBDDQuantumVerifier, QuantumVerifier>(m, "CFLOBDDQuantumVerifier")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &CFLOBDDQuantumVerifier::setNumQubits, "setNumQubits")
        .def("i", &CFLOBDDQuantumVerifier::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &CFLOBDDQuantumVerifier::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &CFLOBDDQuantumVerifier::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &CFLOBDDQuantumVerifier::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &CFLOBDDQuantumVerifier::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &CFLOBDDQuantumVerifier::GetProbability, "GetProbability")
        .def("measure", &CFLOBDDQuantumVerifier::Measure, "Measure")
        .def("y", &CFLOBDDQuantumVerifier::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &CFLOBDDQuantumVerifier::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &CFLOBDDQuantumVerifier::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &CFLOBDDQuantumVerifier::ApplySGate, "ApplySGate")
        .def("t", &CFLOBDDQuantumVerifier::ApplyTGate, "ApplyTGate") 
        .def("cz", &CFLOBDDQuantumVerifier::ApplyCZGate, "ApplyCZGate")
        .def("cx", &CFLOBDDQuantumVerifier::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &CFLOBDDQuantumVerifier::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("gp", &CFLOBDDQuantumVerifier::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &CFLOBDDQuantumVerifier::ApplyCPGate, "ApplyCPGate")
        .def("cs", &CFLOBDDQuantumVerifier::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &CFLOBDDQuantumVerifier::ApplyCSwapGate, "ApplyCSwapGate");

    py::class_<BDDQuantumVerifier, QuantumVerifier>(m, "BDDQuantumVerifier")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &BDDQuantumVerifier::setNumQubits, "setNumQubits")
        .def("i", &BDDQuantumVerifier::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &BDDQuantumVerifier::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &BDDQuantumVerifier::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &BDDQuantumVerifier::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &BDDQuantumVerifier::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &BDDQuantumVerifier::GetProbability, "GetProbability")
        .def("measure", &BDDQuantumVerifier::Measure, "Measure")
        .def("y", &BDDQuantumVerifier::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &BDDQuantumVerifier::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &BDDQuantumVerifier::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &BDDQuantumVerifier::ApplySGate, "ApplySGate")
        .def("t", &BDDQuantumVerifier::ApplyTGate, "ApplyTGate") 
        .def("cz", &BDDQuantumVerifier::ApplyCZGate, "ApplyCZGate")
        .def("cx", &BDDQuantumVerifier::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &BDDQuantumVerifier::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("gp", &BDDQuantumVerifier::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &BDDQuantumVerifier::ApplyCPGate, "ApplyCPGate")
        .def("cs", &BDDQuantumVerifier::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &BDDQuantumVerifier::ApplyCSwapGate, "ApplyCSwapGate");
    
    py::class_<WeightedBDDQuantumVerifier, QuantumVerifier>(m, "WeightedBDDQuantumVerifier")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &WeightedBDDQuantumVerifier::setNumQubits, "setNumQubits")
        .def("i", &WeightedBDDQuantumVerifier::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &WeightedBDDQuantumVerifier::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &WeightedBDDQuantumVerifier::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &WeightedBDDQuantumVerifier::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &WeightedBDDQuantumVerifier::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &WeightedBDDQuantumVerifier::GetProbability, "GetProbability")
        .def("measure", &WeightedBDDQuantumVerifier::Measure, "Measure")
        .def("y", &WeightedBDDQuantumVerifier::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &WeightedBDDQuantumVerifier::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &WeightedBDDQuantumVerifier::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &WeightedBDDQuantumVerifier::ApplySGate, "ApplySGate")
        .def("t", &WeightedBDDQuantumVerifier::ApplyTGate, "ApplyTGate") 
        .def("cz", &WeightedBDDQuantumVerifier::ApplyCZGate, "ApplyCZGate")
        .def("cx", &WeightedBDDQuantumVerifier::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &WeightedBDDQuantumVerifier::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("gp", &WeightedBDDQuantumVerifier::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &WeightedBDDQuantumVerifier::ApplyCPGate, "ApplyCPGate")
        .def("cs", &WeightedBDDQuantumVerifier::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &WeightedBDDQuantumVerifier::ApplyCSwapGate, "ApplyCSwapGate");
}
