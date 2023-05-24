#ifndef _QUANTUM_GATE
#define _QUANTUM_GATE

#include "../MQT_DD/dd_package/include/dd/Package.hpp"
#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"
#include <random>


class QuantumGate {
    public:
        // Constructor
        QuantumGate();
        // Destructor
        virtual ~QuantumGate();
        virtual void Print() = 0;
};

using namespace CFL_OBDD;

class WeightedCFLOBDDQuantumGate : public QuantumGate {
    public:
        WeightedCFLOBDDQuantumGate(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL gate);
        WeightedCFLOBDDQuantumGate();
        ~WeightedCFLOBDDQuantumGate();
        void Print();
        WeightedCFLOBDDQuantumGate GetGate() {return gate;}

    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL gate;
};

using namespace dd;
class MQTDDQuantumGate : public QuantumGate
{
    public:
        // Constructor
        MQTDDQuantumGate(dd::mEdge gate);//, std::unique_ptr<Package<DDPackageConfig>>& ddp);
        MQTDDQuantumGate();
        ~MQTDDQuantumGate();
        void Print();
        mEdge GetGate() {return gate;}
    private:
        mEdge gate;
        // std::unique_ptr<Package<DDPackageConfig>> ddp;
};

#endif
