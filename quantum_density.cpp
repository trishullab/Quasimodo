#include "quantum_density.h"

QuantumDensity::QuantumDensity() {}
QuantumDensity::~QuantumDensity() {}

// ********* CFLOBDDQuantumDensity *********
CFLOBDDQuantumDensity::CFLOBDDQuantumDensity()
{
}

CFLOBDDQuantumDensity::CFLOBDDQuantumDensity(CFLOBDD_COMPLEX_BIG g)
{
    state = g;   
}

CFLOBDDQuantumDensity::~CFLOBDDQuantumDensity()
{
}

void CFLOBDDQuantumDensity::Print()
{
    state.print(std::cout);
}

unsigned int CFLOBDDQuantumDensity::Size()
{
    unsigned int nodeCount = 0, edgeCount = 0;
    unsigned int returnEdgesCount = 0, returnEdgesObjCount = 0;
    state.CountNodesAndEdges(nodeCount, edgeCount, returnEdgesCount, returnEdgesObjCount);
    std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;
    std::cout << "return value map size: " << state.root->rootConnection.returnMapHandle.Size() << std::endl;
    state.root->rootConnection.returnMapHandle.print(std::cout);
    return nodeCount + edgeCount;
}


// ********* BDDQuantumDensity *********
BDDQuantumDensity::BDDQuantumDensity()
{
}
BDDQuantumDensity::BDDQuantumDensity(ADD g)
{
    state = g;   
}

BDDQuantumDensity::~BDDQuantumDensity()
{
}

void BDDQuantumDensity::Print()
{
    state.print(2, 2);
}

unsigned int BDDQuantumDensity::Size()
{
    return state.nodeCount();
}

// ********* WeightedBDDQuantumDensity *********
WeightedBDDQuantumDensity::WeightedBDDQuantumDensity()
{
}

WeightedBDDQuantumDensity::WeightedBDDQuantumDensity(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL g)
{
    state = g;   
}

WeightedBDDQuantumDensity::~WeightedBDDQuantumDensity()
{
}

void WeightedBDDQuantumDensity::Print()
{

}

unsigned int WeightedBDDQuantumDensity::Size()
{
    // unsigned int nodeCount = 0, edgeCount = 0;
    // unsigned int returnEdgesCount = 0, returnEdgesObjCount = 0;
    // state.CountNodesAndEdges(nodeCount, edgeCount, returnEdgesCount, returnEdgesObjCount);
    // std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;
    // std::cout << "return value map size: " << state.root->rootConnection.returnMapHandle.Size() << std::endl;
    // return nodeCount + edgeCount;
    return 0;
}

// ********* WeightedCFLOBDDQuantumDensity *********
WeightedCFLOBDDQuantumDensity::WeightedCFLOBDDQuantumDensity()
{
}

WeightedCFLOBDDQuantumDensity::WeightedCFLOBDDQuantumDensity(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL g)
{
    state = g;   
}

WeightedCFLOBDDQuantumDensity::~WeightedCFLOBDDQuantumDensity()
{
}

void WeightedCFLOBDDQuantumDensity::Print()
{
    state.print(std::cout);
}

unsigned int WeightedCFLOBDDQuantumDensity::Size()
{
    unsigned int nodeCount = 0, edgeCount = 0;
    unsigned int returnEdgesCount = 0, returnEdgesObjCount = 0;
    state.CountNodesAndEdges(nodeCount, edgeCount, returnEdgesCount, returnEdgesObjCount);
    std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;
    std::cout << "return value map size: " << state.root->rootConnection.returnMapHandle.Size() << std::endl;
    state.root->rootConnection.returnMapHandle.print(std::cout);
    return nodeCount + edgeCount;
}


// ********* MQTDDQuantumDensity *********
MQTDDQuantumDensity::MQTDDQuantumDensity()
{
}

MQTDDQuantumDensity::MQTDDQuantumDensity(dd::mEdge g)//, std::unique_ptr<Package<DDPackageConfig>>& d) : ddp(std::move(d))
{
    state = g;   
}

MQTDDQuantumDensity::~MQTDDQuantumDensity()
{
}

void MQTDDQuantumDensity::Print()
{
    // ddp->print(state);
}

unsigned int MQTDDQuantumDensity::Size()
{
    // return ddp->size(state);
    return 0;
}
