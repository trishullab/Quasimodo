#include "quantum_state.h"

QuantumState::QuantumState() {}
QuantumState::~QuantumState() {}

// ********* CFLOBDDQuantumState *********
CFLOBDDQuantumState::CFLOBDDQuantumState()
{
}

CFLOBDDQuantumState::CFLOBDDQuantumState(CFLOBDD_COMPLEX_BIG g)
{
    gate = g;   
}

CFLOBDDQuantumState::~CFLOBDDQuantumState()
{
}

void CFLOBDDQuantumState::Print()
{
    gate.print(std::cout);
}


// ********* BDDQuantumState *********
BDDQuantumState::BDDQuantumState()
{
}
BDDQuantumState::BDDQuantumState(ADD g)
{
    gate = g;   
}

BDDQuantumState::~BDDQuantumState()
{
}

void BDDQuantumState::Print()
{
    gate.print(2, 2);
}

// ********* WeightedBDDQuantumState *********
WeightedBDDQuantumState::WeightedBDDQuantumState()
{
}

WeightedBDDQuantumState::WeightedBDDQuantumState(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL g)
{
    gate = g;   
}

WeightedBDDQuantumState::~WeightedBDDQuantumState()
{
}

void WeightedBDDQuantumState::Print()
{

}
