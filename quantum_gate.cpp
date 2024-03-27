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
