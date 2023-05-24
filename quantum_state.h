#ifndef _QUANTUM_STATE
#define _QUANTUM_STATE

#include "../MQT_DD/dd_package/include/dd/Package.hpp"
#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"
#include <random>


class QuantumState {
    public:
        // Constructor
        QuantumState();
        // Destructor
        virtual ~QuantumState();
        virtual void Print() = 0;
};

using namespace CFL_OBDD;

class WeightedCFLOBDDQuantumState : public QuantumState {
    public:
        WeightedCFLOBDDQuantumState(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state);
        WeightedCFLOBDDQuantumState();
        ~WeightedCFLOBDDQuantumState();
        void Print();
        WeightedCFLOBDDQuantumState GetState() {return state;}

    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state;
};

using namespace dd;
class MQTDDQuantumState : public QuantumState
{
    public:
        // Constructor
        MQTDDQuantumState(dd::vEdge state);//, std::unique_ptr<Package<DDPackageConfig>>& ddp);
        MQTDDQuantumState();
        ~MQTDDQuantumState();
        void Print();
        vEdge GetState() {return state;}
    private:
        vEdge state;
        // std::unique_ptr<Package<DDPackageConfig>> ddp;
};

#endif
