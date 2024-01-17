#ifndef _QUANTUM_GATE
#define _QUANTUM_GATE

#include "dd_package/include/dd/Package.hpp"
#include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
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

class CFLOBDDQuantumGate : public QuantumGate {
    public:
        CFLOBDDQuantumGate(CFLOBDD_COMPLEX_BIG gate);
        CFLOBDDQuantumGate();
        ~CFLOBDDQuantumGate();
        void Print();
        CFLOBDD_COMPLEX_BIG GetGate() {return gate;}

    private:
        CFLOBDD_COMPLEX_BIG gate;
};

#include "cflobdd/cudd-complex-big/cplusplus/cuddObj.hh"

class BDDQuantumGate : public QuantumGate {
    public:
        BDDQuantumGate(ADD gate);
        BDDQuantumGate();
        ~BDDQuantumGate();
        void Print();
        ADD GetGate() {return gate;}
    private:
        ADD gate;
};

#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"

class WeightedBDDQuantumGate : public QuantumGate
{
    public:
        // Constructor
        WeightedBDDQuantumGate(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL gate);
        WeightedBDDQuantumGate();
        ~WeightedBDDQuantumGate();
        void Print();
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL GetGate() {return gate;}
    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL gate;
};

class WeightedCFLOBDDQuantumGate : public QuantumGate {
    public:
        WeightedCFLOBDDQuantumGate(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL gate);
        WeightedCFLOBDDQuantumGate();
        ~WeightedCFLOBDDQuantumGate();
        void Print();
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL GetGate() {return gate;}

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
