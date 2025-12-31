#ifndef _QUANTUM_DENSITY
#define _QUANTUM_DENSITY

#include "dd_package/include/dd/Package.hpp"
#include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"
#include <random>


class QuantumDensity {
    public:
        // Constructor
        QuantumDensity();
        // Destructor
        virtual ~QuantumDensity();
        virtual void Print() = 0;
        virtual unsigned int Size() = 0;
};

using namespace CFL_OBDD;

class CFLOBDDQuantumDensity : public QuantumDensity {
    public:
        CFLOBDDQuantumDensity(CFLOBDD_COMPLEX_BIG state);
        CFLOBDDQuantumDensity();
        ~CFLOBDDQuantumDensity();
        void Print();
        CFLOBDD_COMPLEX_BIG GetDensity() {return state;}
        unsigned int Size();

    private:
        CFLOBDD_COMPLEX_BIG state;
};

#include "cflobdd/cudd-complex-big/cplusplus/cuddObj.hh"

class BDDQuantumDensity : public QuantumDensity {
    public:
        BDDQuantumDensity(ADD state);
        BDDQuantumDensity();
        ~BDDQuantumDensity();
        void Print();
        ADD GetDensity() {return state;}
        unsigned int Size();
    private:
        ADD state;
};

#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"

class WeightedBDDQuantumDensity : public QuantumDensity
{
    public:
        // Constructor
        WeightedBDDQuantumDensity(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state);
        WeightedBDDQuantumDensity();
        ~WeightedBDDQuantumDensity();
        void Print();
        unsigned int Size();
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL GetDensity() {return state;}
    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state;
};

class WeightedCFLOBDDQuantumDensity : public QuantumDensity {
    public:
        WeightedCFLOBDDQuantumDensity(WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state);
        WeightedCFLOBDDQuantumDensity();
        ~WeightedCFLOBDDQuantumDensity();
        void Print();
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL GetDensity() {return state;}
        unsigned int Size();

    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL state;
};

using namespace dd;
class MQTDDQuantumDensity : public QuantumDensity
{
    public:
        // Constructor
        MQTDDQuantumDensity(dd::mEdge state);//, std::unique_ptr<Package<DDPackageConfig>>& ddp);
        MQTDDQuantumDensity();
        ~MQTDDQuantumDensity();
        void Print();
        mEdge GetDensity() {return state;}
        unsigned int Size();
    private:
        mEdge state;
        // std::unique_ptr<Package<DDPackageConfig>> ddp;
};

#endif
