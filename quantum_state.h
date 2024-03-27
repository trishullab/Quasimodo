#ifndef _QUANTUM_STATE
#define _QUANTUM_STATE

#include "dd_package/include/dd/Package.hpp"
#include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
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

class CFLOBDDQuantumState : public QuantumState {
    public:
        CFLOBDDQuantumState(CFLOBDD_COMPLEX_BIG state);
        CFLOBDDQuantumState();
        ~CFLOBDDQuantumState();
        void Print();
        CFLOBDD_COMPLEX_BIG GetState() {return state;}

    private:
        CFLOBDD_COMPLEX_BIG state;
};

#include "cflobdd/cudd-complex-big/cplusplus/cuddObj.hh"

class BDDQuantumState : public QuantumState {
    public:
        BDDQuantumState(ADD state);
        BDDQuantumState();
        ~BDDQuantumState();
        void Print();
        ADD GetState() {return state;}
    private:
        ADD state;
};

#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"

class WeightedBDDQuantumState : public QuantumState
{
    public:
        // Constructor
        WeightedBDDQuantumState(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state);
        WeightedBDDQuantumState();
        ~WeightedBDDQuantumState();
        void Print();
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL GetState() {return state;}
    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state;
};

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
