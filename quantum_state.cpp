#include "quantum_state.h"

QuantumState::QuantumState() {}
QuantumState::~QuantumState() {}

// ********* CFLOBDDQuantumState *********
CFLOBDDQuantumState::CFLOBDDQuantumState()
{
}

CFLOBDDQuantumState::CFLOBDDQuantumState(CFLOBDD_COMPLEX_BIG g)
{
    state = g;   
}

CFLOBDDQuantumState::~CFLOBDDQuantumState()
{
}

void CFLOBDDQuantumState::Print()
{
    state.print(std::cout);
}


// ********* BDDQuantumState *********
BDDQuantumState::BDDQuantumState()
{
}
BDDQuantumState::BDDQuantumState(ADD g)
{
    state = g;   
}

BDDQuantumState::~BDDQuantumState()
{
}

void BDDQuantumState::Print()
{
    state.print(2, 2);
}

// ********* WeightedBDDQuantumState *********
WeightedBDDQuantumState::WeightedBDDQuantumState()
{
}

WeightedBDDQuantumState::WeightedBDDQuantumState(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL g)
{
    state = g;   
}

WeightedBDDQuantumState::~WeightedBDDQuantumState()
{
}

void WeightedBDDQuantumState::Print()
{

}

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
