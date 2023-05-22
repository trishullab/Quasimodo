#ifndef _QUANTUM_STATE
#define _QUANTUM_STATE

#include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
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
        CFLOBDDQuantumState(CFLOBDD_COMPLEX_BIG gate);
        CFLOBDDQuantumState();
        ~CFLOBDDQuantumState();
        void Print();
        CFLOBDD_COMPLEX_BIG GetGate() {return gate;}

    private:
        CFLOBDD_COMPLEX_BIG gate;
};

#include "cflobdd/cudd-complex-big/cplusplus/cuddObj.hh"

class BDDQuantumState : public QuantumState {
    public:
        BDDQuantumState(ADD gate);
        BDDQuantumState();
        ~BDDQuantumState();
        void Print();
        ADD GetGate() {return gate;}
    private:
        ADD gate;
};

#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"

class WeightedBDDQuantumState : public QuantumState
{
    public:
        // Constructor
        WeightedBDDQuantumState(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL gate);
        WeightedBDDQuantumState();
        ~WeightedBDDQuantumState();
        void Print();
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL GetGate() {return gate;}
    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL gate;
};

#endif
