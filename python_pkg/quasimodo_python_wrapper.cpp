#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "../quantum_circuit.h"
#include "../quantum_gate.h"

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
        .def("mcx", &CFLOBDDQuantumCircuit::ApplyMCXGate, "ApplyMCXGate")
        .def("ccp", &CFLOBDDQuantumCircuit::ApplyCCPGate, "ApplyCCPGate")
        .def("gp", &CFLOBDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &CFLOBDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &CFLOBDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &CFLOBDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate")
        .def("cu", &CFLOBDDQuantumCircuit::ApplyCUGate, "ApplyCUGate")
        .def("size", &CFLOBDDQuantumCircuit::Size, "Size")
        .def("measure_and_collapse", &CFLOBDDQuantumCircuit::MeasureAndCollapse, "MeasureAndCollapse")
        .def("create_h", &CFLOBDDQuantumCircuit::CreateHadamardGate, "CreateHadamardGate")
        .def("create_i", &CFLOBDDQuantumCircuit::CreateIdentityGate, "CreateIdentityGate")
        .def("create_x", &CFLOBDDQuantumCircuit::CreateNOTGate, "CreateNOTGate")
        .def("create_cx", &CFLOBDDQuantumCircuit::CreateCNOTGate, "CreateCNOTGate")
        .def("gate_gate_apply", &CFLOBDDQuantumCircuit::GateGateApply, "GateGateApply")
        .def("apply_gate", &CFLOBDDQuantumCircuit::ApplyGate, "ApplyGate")
        .def("get_state", &CFLOBDDQuantumCircuit::GetState, "GetState");

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
        .def("mcx", &BDDQuantumCircuit::ApplyMCXGate, "ApplyMCXGate")
        .def("ccp", &BDDQuantumCircuit::ApplyCCPGate, "ApplyCCPGate")
        .def("gp", &BDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &BDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &BDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &BDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate")
        .def("cu", &BDDQuantumCircuit::ApplyCUGate, "ApplyCUGate")
        .def("size", &BDDQuantumCircuit::Size, "Size")
        .def("measure_and_collapse", &BDDQuantumCircuit::MeasureAndCollapse, "MeasureAndCollapse")
        .def("create_h", &BDDQuantumCircuit::CreateHadamardGate, "CreateHadamardGate")
        .def("create_i", &BDDQuantumCircuit::CreateIdentityGate, "CreateIdentityGate")
        .def("create_x", &BDDQuantumCircuit::CreateNOTGate, "CreateNOTGate")
        .def("create_cx", &BDDQuantumCircuit::CreateCNOTGate, "CreateCNOTGate")
        .def("gate_gate_apply", &BDDQuantumCircuit::GateGateApply, "GateGateApply")
        .def("apply_gate", &BDDQuantumCircuit::ApplyGate, "ApplyGate")
        .def("get_state", &BDDQuantumCircuit::GetState, "GetState");
    
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
        .def("mcx", &WeightedBDDQuantumCircuit::ApplyMCXGate, "ApplyMCXGate")
        .def("ccp", &WeightedBDDQuantumCircuit::ApplyCCPGate, "ApplyCCPGate")
        .def("gp", &WeightedBDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &WeightedBDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &WeightedBDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &WeightedBDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate")
        .def("size", &WeightedBDDQuantumCircuit::Size, "Size")
        .def("measure_and_collapse", &WeightedBDDQuantumCircuit::MeasureAndCollapse, "MeasureAndCollapse")
        .def("create_h", &WeightedBDDQuantumCircuit::CreateHadamardGate, "CreateHadamardGate")
        .def("create_i", &WeightedBDDQuantumCircuit::CreateIdentityGate, "CreateIdentityGate")
        .def("create_x", &WeightedBDDQuantumCircuit::CreateNOTGate, "CreateNOTGate")
        .def("create_cx", &WeightedBDDQuantumCircuit::CreateCNOTGate, "CreateCNOTGate")
        .def("gate_gate_apply", &WeightedBDDQuantumCircuit::GateGateApply, "GateGateApply")
        .def("apply_gate", &WeightedBDDQuantumCircuit::ApplyGate, "ApplyGate")
        .def("get_state", &WeightedBDDQuantumCircuit::GetState, "GetState");


    py::class_<QuantumGate>(m, "QuantumGate");
    
    py::class_<CFLOBDDQuantumGate, QuantumGate>(m, "CFLOBDDQuantumGate")
        .def(py::init<>())
        .def("print", &CFLOBDDQuantumGate::Print, "Print");
    
    py::class_<BDDQuantumGate, QuantumGate>(m, "BDDQuantumGate")
        .def(py::init<>())
        .def("print", &BDDQuantumGate::Print, "Print");

    py::class_<WeightedBDDQuantumGate, QuantumGate>(m, "WeightedBDDQuantumGate")
        .def(py::init<>())
        .def("print", &WeightedBDDQuantumGate::Print, "Print");

    py::class_<QuantumState>(m, "QuantumState");
    
    py::class_<CFLOBDDQuantumState, QuantumState>(m, "CFLOBDDQuantumState")
        .def(py::init<>())
        .def("print", &CFLOBDDQuantumState::Print, "Print");
    
    py::class_<BDDQuantumState, QuantumState>(m, "BDDQuantumState")
        .def(py::init<>())
        .def("print", &BDDQuantumState::Print, "Print");

    py::class_<WeightedBDDQuantumState, QuantumState>(m, "WeightedBDDQuantumState")
        .def(py::init<>())
        .def("print", &WeightedBDDQuantumState::Print, "Print");
    
}
