#include "quantum_state.h"

QuantumState::QuantumState() {}
QuantumState::~QuantumState() {}

// ********* WeightedCFLOBDDQuantumState *********
WeightedCFLOBDDQuantumState::WeightedCFLOBDDQuantumState()
{
}

WeightedCFLOBDDQuantumState::WeightedCFLOBDDQuantumState(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL g)
{
    state = g;   
}

WeightedCFLOBDDQuantumState::~WeightedCFLOBDDQuantumState()
{
}

void WeightedCFLOBDDQuantumState::Print()
{
    state.print(std::cout);
}


// ********* MQTDDQuantumState *********
MQTDDQuantumState::MQTDDQuantumState()
{
}

MQTDDQuantumState::MQTDDQuantumState(dd::vEdge g)//, std::unique_ptr<Package<DDPackageConfig>>& d) : ddp(std::move(d))
{
    state = g;   
}

MQTDDQuantumState::~MQTDDQuantumState()
{
}

void MQTDDQuantumState::Print()
{
    // ddp->printVector(state);
}
