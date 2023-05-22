#include "quantum_gate.h"

QuantumGate::QuantumGate() {}
QuantumGate::~QuantumGate() {}

// ********* CFLOBDDQuantumGate *********
CFLOBDDQuantumGate::CFLOBDDQuantumGate()
{
}

CFLOBDDQuantumGate::CFLOBDDQuantumGate(CFLOBDD_COMPLEX_BIG g)
{
    gate = g;   
}

CFLOBDDQuantumGate::~CFLOBDDQuantumGate()
{
}

void CFLOBDDQuantumGate::Print()
{
    gate.print(std::cout);
}


// ********* BDDQuantumGate *********
BDDQuantumGate::BDDQuantumGate()
{
}
BDDQuantumGate::BDDQuantumGate(ADD g)
{
    gate = g;   
}

BDDQuantumGate::~BDDQuantumGate()
{
}

void BDDQuantumGate::Print()
{
    gate.print(2, 2);
}

// ********* WeightedBDDQuantumGate *********
WeightedBDDQuantumGate::WeightedBDDQuantumGate()
{
}

WeightedBDDQuantumGate::WeightedBDDQuantumGate(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL g)
{
    gate = g;   
}

WeightedBDDQuantumGate::~WeightedBDDQuantumGate()
{
}

void WeightedBDDQuantumGate::Print()
{

}
