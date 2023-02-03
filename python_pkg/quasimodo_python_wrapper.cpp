#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "../quantum_circuit.h"

namespace py = pybind11;

PYBIND11_MODULE(pyquasimodo, m) {
    m.doc() = "python wrapper for Quantum Simulation"; // Optional module docstring

    py::class_<QuantumCircuit>(m, "QuantumCircuit");
        // .def(py::init<>())
        // .def(py::init<unsigned int>());
    
    py::class_<CFLOBDDQuantumCircuit, QuantumCircuit>(m, "CFLOBDDQuantumCircuit")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &CFLOBDDQuantumCircuit::setNumQubits, "setNumQubits")
        .def("i", &CFLOBDDQuantumCircuit::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &CFLOBDDQuantumCircuit::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &CFLOBDDQuantumCircuit::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &CFLOBDDQuantumCircuit::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &CFLOBDDQuantumCircuit::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &CFLOBDDQuantumCircuit::GetProbability, "GetProbability")
        .def("measure", &CFLOBDDQuantumCircuit::Measure, "Measure")
        .def("measurement_counts", &CFLOBDDQuantumCircuit::GetPathCount, "MeasurementCount")
        .def("y", &CFLOBDDQuantumCircuit::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &CFLOBDDQuantumCircuit::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &CFLOBDDQuantumCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &CFLOBDDQuantumCircuit::ApplySGate, "ApplySGate")
        .def("t", &CFLOBDDQuantumCircuit::ApplyTGate, "ApplyTGate") 
        .def("cz", &CFLOBDDQuantumCircuit::ApplyCZGate, "ApplyCZGate")
        .def("cx", &CFLOBDDQuantumCircuit::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &CFLOBDDQuantumCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("gp", &CFLOBDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &CFLOBDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &CFLOBDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &CFLOBDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate");

    py::class_<BDDQuantumCircuit, QuantumCircuit>(m, "BDDQuantumCircuit")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &BDDQuantumCircuit::setNumQubits, "setNumQubits")
        .def("i", &BDDQuantumCircuit::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &BDDQuantumCircuit::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &BDDQuantumCircuit::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &BDDQuantumCircuit::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &BDDQuantumCircuit::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &BDDQuantumCircuit::GetProbability, "GetProbability")
        .def("measure", &BDDQuantumCircuit::Measure, "Measure")
        .def("measurement_counts", &BDDQuantumCircuit::GetPathCount, "MeasurementCount")
        .def("y", &BDDQuantumCircuit::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &BDDQuantumCircuit::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &BDDQuantumCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &BDDQuantumCircuit::ApplySGate, "ApplySGate")
        .def("t", &BDDQuantumCircuit::ApplyTGate, "ApplyTGate") 
        .def("cz", &BDDQuantumCircuit::ApplyCZGate, "ApplyCZGate")
        .def("cx", &BDDQuantumCircuit::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &BDDQuantumCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("gp", &BDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &BDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &BDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &BDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate");
    
    py::class_<WeightedBDDQuantumCircuit, QuantumCircuit>(m, "WeightedBDDQuantumCircuit")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &WeightedBDDQuantumCircuit::setNumQubits, "setNumQubits")
        .def("i", &WeightedBDDQuantumCircuit::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &WeightedBDDQuantumCircuit::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &WeightedBDDQuantumCircuit::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &WeightedBDDQuantumCircuit::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &WeightedBDDQuantumCircuit::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &WeightedBDDQuantumCircuit::GetProbability, "GetProbability")
        .def("measure", &WeightedBDDQuantumCircuit::Measure, "Measure")
        .def("measurement_counts", &WeightedBDDQuantumCircuit::GetPathCount, "MeasurementCount")
        .def("y", &WeightedBDDQuantumCircuit::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &WeightedBDDQuantumCircuit::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &WeightedBDDQuantumCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &WeightedBDDQuantumCircuit::ApplySGate, "ApplySGate")
        .def("t", &WeightedBDDQuantumCircuit::ApplyTGate, "ApplyTGate") 
        .def("cz", &WeightedBDDQuantumCircuit::ApplyCZGate, "ApplyCZGate")
        .def("cx", &WeightedBDDQuantumCircuit::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &WeightedBDDQuantumCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("gp", &WeightedBDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &WeightedBDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &WeightedBDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &WeightedBDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate");
}
