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

unsigned int CFLOBDDQuantumState::Size()
{
    unsigned int nodeCount = 0, edgeCount = 0;
    unsigned int returnEdgesCount = 0, returnEdgesObjCount = 0;
    state.CountNodesAndEdges(nodeCount, edgeCount, returnEdgesCount, returnEdgesObjCount);
    std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;
    std::cout << "return value map size: " << state.root->rootConnection.returnMapHandle.Size() << std::endl;
    state.root->rootConnection.returnMapHandle.print(std::cout);
    return nodeCount + edgeCount;
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

unsigned int BDDQuantumState::Size()
{
    return state.nodeCount();
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

unsigned int WeightedBDDQuantumState::Size()
{
    // unsigned int nodeCount = 0, edgeCount = 0;
    // unsigned int returnEdgesCount = 0, returnEdgesObjCount = 0;
    // state.CountNodesAndEdges(nodeCount, edgeCount, returnEdgesCount, returnEdgesObjCount);
    // std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;
    // std::cout << "return value map size: " << state.root->rootConnection.returnMapHandle.Size() << std::endl;
    // return nodeCount + edgeCount;
    return 0;
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

unsigned int WeightedCFLOBDDQuantumState::Size()
{
    unsigned int nodeCount = 0, edgeCount = 0;
    unsigned int returnEdgesCount = 0, returnEdgesObjCount = 0;
    state.CountNodesAndEdges(nodeCount, edgeCount, returnEdgesCount, returnEdgesObjCount);
    std::cout << "Node Count: " << nodeCount << ", Edge Count: " << edgeCount << std::endl;
    std::cout << "return value map size: " << state.root->rootConnection.returnMapHandle.Size() << std::endl;
    state.root->rootConnection.returnMapHandle.print(std::cout);
    return nodeCount + edgeCount;
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

unsigned int MQTDDQuantumState::Size()
{
    // return ddp->size(state);
    return 0;
}
