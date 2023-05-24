#include "quantum_gate.h"

QuantumGate::QuantumGate() {}
QuantumGate::~QuantumGate() {}

// ********* WeightedCFLOBDDQuantumGate *********
WeightedCFLOBDDQuantumGate::WeightedCFLOBDDQuantumGate()
{
}

WeightedCFLOBDDQuantumGate::WeightedCFLOBDDQuantumGate(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL g)
{
    gate = g;   
}

WeightedCFLOBDDQuantumGate::~WeightedCFLOBDDQuantumGate()
{
}

void WeightedCFLOBDDQuantumGate::Print()
{
    gate.print(std::cout);
}


// ********* MQTDDQuantumGate *********
MQTDDQuantumGate::MQTDDQuantumGate()
{
}

MQTDDQuantumGate::MQTDDQuantumGate(dd::mEdge g)//, std::unique_ptr<Package<DDPackageConfig>>& d) : ddp(std::move(d))
{
    gate = g;   
}

MQTDDQuantumGate::~MQTDDQuantumGate()
{
}

void MQTDDQuantumGate::Print()
{
    // ddp->printMatrix(gate);
}
