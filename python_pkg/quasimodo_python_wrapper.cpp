#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <vector>
#include "../quantum_circuit.h"
#include "../quantum_gate.h"
#include "../quantum_state.h"

namespace py = pybind11;

PYBIND11_MODULE(pyquasimodo, m) {
    m.doc() = "python wrapper for Quantum Simulation"; // Optional module docstring

    py::class_<QuantumCircuit>(m, "QuantumCircuit");
        // .def(py::init<>())
        // .def(py::init<unsigned int>());
    
    // py::class_<CFLOBDDQuantumCircuit, QuantumCircuit>(m, "CFLOBDDQuantumCircuit")
    //     .def(py::init<>())
    //     .def(py::init<unsigned int, int>())
    //     .def("setNumQubits", &CFLOBDDQuantumCircuit::setNumQubits, "setNumQubits")
    //     .def("i", &CFLOBDDQuantumCircuit::ApplyIdentityGate, "ApplyIdentityGate")
    //     .def("h", &CFLOBDDQuantumCircuit::ApplyHadamardGate, "ApplyHadamardGate")
    //     .def("x", &CFLOBDDQuantumCircuit::ApplyNOTGate, "ApplyNOTGate")
    //     .def("swap", &CFLOBDDQuantumCircuit::ApplySwapGate, "ApplySwapGate")
    //     .def("iswap", &CFLOBDDQuantumCircuit::ApplyiSwapGate, "ApplyiSwapGate")
    //     .def("prob", &CFLOBDDQuantumCircuit::GetProbability, "GetProbability")
    //     .def("measure", &CFLOBDDQuantumCircuit::Measure, "Measure")
    //     .def("measurement_counts", &CFLOBDDQuantumCircuit::GetPathCount, "MeasurementCount")
    //     .def("y", &CFLOBDDQuantumCircuit::ApplyPauliYGate, "ApplyPauliYGate")
    //     .def("z", &CFLOBDDQuantumCircuit::ApplyPauliZGate, "ApplyPauliZGate")
    //     .def("p", &CFLOBDDQuantumCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
    //     .def("s", &CFLOBDDQuantumCircuit::ApplySGate, "ApplySGate")
    //     .def("t", &CFLOBDDQuantumCircuit::ApplyTGate, "ApplyTGate") 
    //     .def("cz", &CFLOBDDQuantumCircuit::ApplyCZGate, "ApplyCZGate")
    //     .def("cx", &CFLOBDDQuantumCircuit::ApplyCNOTGate, "ApplyCNOTGate")
    //     .def("ccx", &CFLOBDDQuantumCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
    //     .def("gp", &CFLOBDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
    //     .def("cp", &CFLOBDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
    //     .def("cs", &CFLOBDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
    //     .def("cswap", &CFLOBDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate");

    // py::class_<BDDQuantumCircuit, QuantumCircuit>(m, "BDDQuantumCircuit")
    //     .def(py::init<>())
    //     .def(py::init<unsigned int, int>())
    //     .def("setNumQubits", &BDDQuantumCircuit::setNumQubits, "setNumQubits")
    //     .def("i", &BDDQuantumCircuit::ApplyIdentityGate, "ApplyIdentityGate")
    //     .def("h", &BDDQuantumCircuit::ApplyHadamardGate, "ApplyHadamardGate")
    //     .def("x", &BDDQuantumCircuit::ApplyNOTGate, "ApplyNOTGate")
    //     .def("swap", &BDDQuantumCircuit::ApplySwapGate, "ApplySwapGate")
    //     .def("iswap", &BDDQuantumCircuit::ApplyiSwapGate, "ApplyiSwapGate")
    //     .def("prob", &BDDQuantumCircuit::GetProbability, "GetProbability")
    //     .def("measure", &BDDQuantumCircuit::Measure, "Measure")
    //     .def("measurement_counts", &BDDQuantumCircuit::GetPathCount, "MeasurementCount")
    //     .def("y", &BDDQuantumCircuit::ApplyPauliYGate, "ApplyPauliYGate")
    //     .def("z", &BDDQuantumCircuit::ApplyPauliZGate, "ApplyPauliZGate")
    //     .def("p", &BDDQuantumCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
    //     .def("s", &BDDQuantumCircuit::ApplySGate, "ApplySGate")
    //     .def("t", &BDDQuantumCircuit::ApplyTGate, "ApplyTGate") 
    //     .def("cz", &BDDQuantumCircuit::ApplyCZGate, "ApplyCZGate")
    //     .def("cx", &BDDQuantumCircuit::ApplyCNOTGate, "ApplyCNOTGate")
    //     .def("ccx", &BDDQuantumCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
    //     .def("gp", &BDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
    //     .def("cp", &BDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
    //     .def("cs", &BDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
    //     .def("cswap", &BDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate");
    
    // py::class_<WeightedBDDQuantumCircuit, QuantumCircuit>(m, "WeightedBDDQuantumCircuit")
    //     .def(py::init<>())
    //     .def(py::init<unsigned int, int>())
    //     .def("setNumQubits", &WeightedBDDQuantumCircuit::setNumQubits, "setNumQubits")
    //     .def("i", &WeightedBDDQuantumCircuit::ApplyIdentityGate, "ApplyIdentityGate")
    //     .def("h", &WeightedBDDQuantumCircuit::ApplyHadamardGate, "ApplyHadamardGate")
    //     .def("x", &WeightedBDDQuantumCircuit::ApplyNOTGate, "ApplyNOTGate")
    //     .def("swap", &WeightedBDDQuantumCircuit::ApplySwapGate, "ApplySwapGate")
    //     .def("iswap", &WeightedBDDQuantumCircuit::ApplyiSwapGate, "ApplyiSwapGate")
    //     .def("prob", &WeightedBDDQuantumCircuit::GetProbability, "GetProbability")
    //     .def("measure", &WeightedBDDQuantumCircuit::Measure, "Measure")
    //     .def("measurement_counts", &WeightedBDDQuantumCircuit::GetPathCount, "MeasurementCount")
    //     .def("y", &WeightedBDDQuantumCircuit::ApplyPauliYGate, "ApplyPauliYGate")
    //     .def("z", &WeightedBDDQuantumCircuit::ApplyPauliZGate, "ApplyPauliZGate")
    //     .def("p", &WeightedBDDQuantumCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
    //     .def("s", &WeightedBDDQuantumCircuit::ApplySGate, "ApplySGate")
    //     .def("t", &WeightedBDDQuantumCircuit::ApplyTGate, "ApplyTGate") 
    //     .def("cz", &WeightedBDDQuantumCircuit::ApplyCZGate, "ApplyCZGate")
    //     .def("cx", &WeightedBDDQuantumCircuit::ApplyCNOTGate, "ApplyCNOTGate")
    //     .def("ccx", &WeightedBDDQuantumCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
    //     .def("gp", &WeightedBDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
    //     .def("cp", &WeightedBDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
    //     .def("cs", &WeightedBDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
    //     .def("cswap", &WeightedBDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate");

    py::class_<WeightedCFLOBDDQuantumCircuit, QuantumCircuit>(m, "WeightedCFLOBDDQuantumCircuit")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &WeightedCFLOBDDQuantumCircuit::setNumQubits, "setNumQubits")
        .def("i", &WeightedCFLOBDDQuantumCircuit::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &WeightedCFLOBDDQuantumCircuit::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &WeightedCFLOBDDQuantumCircuit::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &WeightedCFLOBDDQuantumCircuit::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &WeightedCFLOBDDQuantumCircuit::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &WeightedCFLOBDDQuantumCircuit::GetProbability, "GetProbability")
        .def("measure", &WeightedCFLOBDDQuantumCircuit::Measure, "Measure")
        .def("measurement_counts", &WeightedCFLOBDDQuantumCircuit::GetPathCount, "MeasurementCount")
        .def("y", &WeightedCFLOBDDQuantumCircuit::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &WeightedCFLOBDDQuantumCircuit::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &WeightedCFLOBDDQuantumCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &WeightedCFLOBDDQuantumCircuit::ApplySGate, "ApplySGate")
        .def("t", &WeightedCFLOBDDQuantumCircuit::ApplyTGate, "ApplyTGate") 
        .def("cz", &WeightedCFLOBDDQuantumCircuit::ApplyCZGate, "ApplyCZGate")
        .def("cx", &WeightedCFLOBDDQuantumCircuit::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &WeightedCFLOBDDQuantumCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("mcx", &WeightedCFLOBDDQuantumCircuit::ApplyMCXGate, "ApplyMCXGate")
        .def("gp", &WeightedCFLOBDDQuantumCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &WeightedCFLOBDDQuantumCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &WeightedCFLOBDDQuantumCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &WeightedCFLOBDDQuantumCircuit::ApplyCSwapGate, "ApplyCSwapGate")
        .def("sx", &WeightedCFLOBDDQuantumCircuit::ApplySXGate, "ApplySXGate")
        .def("sy", &WeightedCFLOBDDQuantumCircuit::ApplySYGate, "ApplySYGate")
        .def("ccp", &WeightedCFLOBDDQuantumCircuit::ApplyCCPGate, "ApplyCCPGate")
        .def("measure_and_collapse", &WeightedCFLOBDDQuantumCircuit::MeasureAndCollapse, "MeasureAndCollapse")
        .def("size", &WeightedCFLOBDDQuantumCircuit::Size, "Size")
        .def("create_h", &WeightedCFLOBDDQuantumCircuit::CreateHadamardGate, "CreateHadamardGate")
        .def("create_i", &WeightedCFLOBDDQuantumCircuit::CreateIdentityGate, "CreateIdentityGate")
        .def("create_x", &WeightedCFLOBDDQuantumCircuit::CreateNOTGate, "CreateNOTGate")
        .def("create_cx", &WeightedCFLOBDDQuantumCircuit::CreateCNOTGate, "CreateCNOTGate")
        .def("gate_gate_apply", &WeightedCFLOBDDQuantumCircuit::GateGateApply, "GateGateApply")
        .def("apply_gate", &WeightedCFLOBDDQuantumCircuit::ApplyGate, "ApplyGate")
        .def("get_state", &WeightedCFLOBDDQuantumCircuit::GetState, "GetState");

    
    py::class_<MQTDDCircuit, QuantumCircuit>(m, "MQTDDCircuit")
        .def(py::init<>())
        .def(py::init<unsigned int, int>())
        .def("setNumQubits", &MQTDDCircuit::setNumQubits, "setNumQubits")
        .def("i", &MQTDDCircuit::ApplyIdentityGate, "ApplyIdentityGate")
        .def("h", &MQTDDCircuit::ApplyHadamardGate, "ApplyHadamardGate")
        .def("x", &MQTDDCircuit::ApplyNOTGate, "ApplyNOTGate")
        .def("swap", &MQTDDCircuit::ApplySwapGate, "ApplySwapGate")
        .def("iswap", &MQTDDCircuit::ApplyiSwapGate, "ApplyiSwapGate")
        .def("prob", &MQTDDCircuit::GetProbability, "GetProbability")
        .def("measure", &MQTDDCircuit::Measure, "Measure")
        .def("measurement_counts", &MQTDDCircuit::GetPathCount, "MeasurementCount")
        .def("y", &MQTDDCircuit::ApplyPauliYGate, "ApplyPauliYGate")
        .def("z", &MQTDDCircuit::ApplyPauliZGate, "ApplyPauliZGate")
        .def("p", &MQTDDCircuit::ApplyPhaseShiftGate, "ApplyPhaseShiftGate")
        .def("s", &MQTDDCircuit::ApplySGate, "ApplySGate")
        .def("t", &MQTDDCircuit::ApplyTGate, "ApplyTGate")
        .def("cz", &MQTDDCircuit::ApplyCZGate, "ApplyCZGate")
        .def("cx", &MQTDDCircuit::ApplyCNOTGate, "ApplyCNOTGate")
        .def("ccx", &MQTDDCircuit::ApplyCCNOTGate, "ApplyCCNOTGate")
        .def("mcx", &MQTDDCircuit::ApplyMCXGate, "ApplyMCXGate")
        .def("gp", &MQTDDCircuit::ApplyGlobalPhase, "ApplyGlobalPhase")
        .def("cp", &MQTDDCircuit::ApplyCPGate, "ApplyCPGate")
        .def("cs", &MQTDDCircuit::ApplyCSGate, "ApplyCSGate")
        .def("cswap", &MQTDDCircuit::ApplyCSwapGate, "ApplyCSwapGate")
        .def("sx", &MQTDDCircuit::ApplySXGate, "ApplySXGate")
        .def("sy", &MQTDDCircuit::ApplySYGate, "ApplySYGate")
        .def("ccp", &MQTDDCircuit::ApplyCCPGate, "ApplyCCPGate")
        .def("measure_and_collapse", &MQTDDCircuit::MeasureAndCollapse, "MeasureAndCollapse")
        .def("size", &MQTDDCircuit::Size, "Size")
        .def("create_h", &MQTDDCircuit::CreateHadamardGate, "CreateHadamardGate")
        .def("create_i", &MQTDDCircuit::CreateIdentityGate, "CreateIdentityGate")
        .def("create_x", &MQTDDCircuit::CreateNOTGate, "CreateNOTGate")
        .def("create_cx", &MQTDDCircuit::CreateCNOTGate, "CreateCNOTGate")
        .def("gate_gate_apply", &MQTDDCircuit::GateGateApply, "GateGateApply")
        .def("apply_gate", &MQTDDCircuit::ApplyGate, "ApplyGate")
        .def("get_state", &MQTDDCircuit::GetState, "GetState");
        

    
    py::class_<QuantumGate>(m, "QuantumGate");
    
    py::class_<WeightedCFLOBDDQuantumGate, QuantumGate>(m, "WeightedCFLOBDDQuantumGate")
        .def(py::init<>())
        .def("print", &WeightedCFLOBDDQuantumGate::Print, "Print");
    
    py::class_<MQTDDQuantumGate, QuantumGate>(m, "MQTDDQuantumGate")
        .def(py::init<>())
        .def("print", &MQTDDQuantumGate::Print, "Print");

    py::class_<QuantumState>(m, "QuantumState");
    
    py::class_<WeightedCFLOBDDQuantumState, QuantumState>(m, "WeightedCFLOBDDQuantumState")
        .def(py::init<>())
        .def("print", &WeightedCFLOBDDQuantumState::Print, "Print");
    
    py::class_<MQTDDQuantumState, QuantumState>(m, "MQTDDQuantumState")
        .def(py::init<>())
        .def("print", &MQTDDQuantumState::Print, "Print");

}
