#include "quantum_verifier.h"
#include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
#include "cflobdd/CFLOBDD/vector_complex_float_boost.h"
#include <random>

QuantumVerifier::QuantumVerifier(unsigned int numQubits, int seed) :  numQubits (numQubits) 
{
    std::mt19937 mt(seed);
    srand(seed);
    hadamard_count = 0;
}
QuantumVerifier::QuantumVerifier() :  numQubits (0), hadamard_count (0) {}
QuantumVerifier::~QuantumVerifier() {}

// using namespace CFL_OBDD;

CFLOBDDQuantumVerifier::CFLOBDDQuantumVerifier(unsigned int numQubits, int seed) : QuantumVerifier(numQubits, seed)
{
    // Initialize
        CFLOBDDNodeHandle::InitNoDistinctionTable();
        CFLOBDDNodeHandle::InitAdditionInterleavedTable();
        CFLOBDDNodeHandle::InitReduceCache();
        InitPairProductCache();
        InitTripleProductCache();
        Matrix1234ComplexFloatBoost::Matrix1234Initializer();
        VectorComplexFloatBoost::VectorInitializer();
    //
    unsigned int level = ceil(log2(numQubits)) + 1;
    stateVector = VectorComplexFloatBoost::MkBasisVector(level, 0);
}

CFLOBDDQuantumVerifier::CFLOBDDQuantumVerifier()
{
    // Initialize
        CFLOBDDNodeHandle::InitNoDistinctionTable();
        CFLOBDDNodeHandle::InitAdditionInterleavedTable();
        CFLOBDDNodeHandle::InitReduceCache();
        InitPairProductCache();
        InitTripleProductCache();
        Matrix1234ComplexFloatBoost::Matrix1234Initializer();
        VectorComplexFloatBoost::VectorInitializer();
        numQubits = 0;
    //
}

CFLOBDDQuantumVerifier::~CFLOBDDQuantumVerifier()
{
    DisposeOfTripleProductCache();
	DisposeOfPairProductCache();
	CFLOBDDNodeHandle::DisposeOfReduceCache();
}

void CFLOBDDQuantumVerifier::setNumQubits(unsigned int num)
{
    numQubits = num;
    unsigned int level = ceil(log2(numQubits)) + 1;
    stateVector = VectorComplexFloatBoost::MkBasisVector(level, 0); 
}

CFLOBDD_COMPLEX_BIG ApplyGateF(unsigned int n, unsigned int i, CFLOBDD_COMPLEX_BIG(*f)(unsigned int))
{
    if (n == 1)
    {
        return f(1);
    }
    else {
        int level = ceil(log2(n/2));
        if (i < n/2)
        {
            CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
            CFLOBDD_COMPLEX_BIG H = ApplyGateF(n/2, i, f);
            return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(H, T);
        }
        else
        {
            CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
            return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(T, ApplyGateF(n/2, i - n/2, f)); 
        }
    }
}

CFLOBDD_COMPLEX_BIG ApplyGateFWithParam(unsigned int n, unsigned int i, CFLOBDD_COMPLEX_BIG(*f)(unsigned int, double), double theta)
{
    if (n == 1)
    {
        return f(1, theta);
    }
    else {
        int level = ceil(log2(n/2));
        if (i < n/2)
        {
            CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
            CFLOBDD_COMPLEX_BIG H = ApplyGateFWithParam(n/2, i, f, theta);
            return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(H, T);
        }
        else
        {
            CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
            return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(T, ApplyGateFWithParam(n/2, i - n/2, f, theta)); 
        }
    }
}

bool checkForInit(unsigned int numQubits)
{
    return numQubits != 0;
}

void CFLOBDDQuantumVerifier::ApplyIdentityGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto H = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkIdRelationInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(H, stateVector);
}

void CFLOBDDQuantumVerifier::ApplyHadamardGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto H = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkWalshInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(H, stateVector);
    hadamard_count++;
}

void CFLOBDDQuantumVerifier::ApplyNOTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto X = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkNegationMatrixInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(X, stateVector);
}

void CFLOBDDQuantumVerifier::ApplyPauliYGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto Y = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPauliYMatrixInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(Y, stateVector);
}

void CFLOBDDQuantumVerifier::ApplyPauliZGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto Z = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPauliZMatrixInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(Z, stateVector);
}

void CFLOBDDQuantumVerifier::ApplySGate(unsigned int index)
{
    auto S = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkSGateInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector);
}

void CFLOBDDQuantumVerifier::ApplyCNOTGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = Matrix1234ComplexFloatBoost::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller, controlled);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else
    {
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = Matrix1234ComplexFloatBoost::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
}

void CFLOBDDQuantumVerifier::ApplySwapGate(long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(index1 != index2);
    
    if (index1 < index2)
    {
        auto C = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index1, index2);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else
    {
        auto C = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index2, index1);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
}

void CFLOBDDQuantumVerifier::ApplyiSwapGate(long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(index1 != index2);
    
    if (index1 < index2)
    {
        auto C = Matrix1234ComplexFloatBoost::MkiSwapGate(stateVector.root->level, index1, index2);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else
    {
        auto C = Matrix1234ComplexFloatBoost::MkiSwapGate(stateVector.root->level, index2, index1);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
}

void CFLOBDDQuantumVerifier::ApplyCZGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controller, controlled, 1.0);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else
    {
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controlled, controller, 1.0);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
}

void CFLOBDDQuantumVerifier::ApplyCPGate(long int controller, long int controlled, double theta)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controller, controlled, theta);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else
    {
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controlled, controller, theta);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    } 
}

void CFLOBDDQuantumVerifier::ApplyCSGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ApplyCPGate(controller, controlled, 0.5);
}

void CFLOBDDQuantumVerifier::ApplyPhaseShiftGate(unsigned int index, double theta)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPhaseShiftGateInterleaved, theta);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector); 
}

void CFLOBDDQuantumVerifier::ApplyTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPhaseShiftGateInterleaved, 0.25);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector); 
}

void CFLOBDDQuantumVerifier::ApplyCCNOTGate(long int controller1, long int controller2, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller1 != controlled);
    assert(controller2 != controlled);
    assert(controller1 != controller2);
    if (controller1 < controller2 && controller2 < controlled)
    {
        // a b c
        auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controller2, controlled);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (controller1 < controlled && controlled < controller2)
    {
        // a c b   
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller2);
        auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controlled, controller2);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (controller2 < controller1 && controller1 < controlled)
    {
        // b a c
        ApplyCCNOTGate(controller2, controller1, controlled);
    }
    else if (controller2 < controlled && controlled < controller1)
    {
        // b c a
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller1);
        auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller2, controlled, controller1);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector); 
    }
    else if (controlled < controller1 && controller1 < controller2)
    {
        // c a b
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller2);
        // b a c
        auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller1, controller2);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (controlled < controller2 && controller2 < controller1)
    {
        // c b a
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller1);
        // a b c
        auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller2, controller1);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
}

void CFLOBDDQuantumVerifier::ApplyCSwapGate(long int controller, long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller != index1);
    assert(controller != index2);
    assert(index1 != index2);
    
    if (controller < index1 && index1 < index2)
    {
        // a b c
        auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, controller, index1, index2);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (controller < index2 && index2 < index1)
    {
        // a c b   
        auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, controller, index2, index1);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (index1 < controller && controller < index2)
    {
        // b a c
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index1, controller);
        auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), index1, controller, index2);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (index1 < index2 && index2 < controller)
    {
        // b c a
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index1, controller);
        auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index1, index2, controller);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (index2 < controller && controller < index1)
    {
        // c a b
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index2, controller);
        // b a c
        auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index2, controller, index1);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (index2 < index1 && index1 < controller)
    {
        // c b a
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index2, controller);
        // a b c
        auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index2, index1, controller);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
}

void CFLOBDDQuantumVerifier::ApplyGlobalPhase(double phase)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto cos_v = boost::math::cos_pi(phase);
    auto sin_v = boost::math::sin_pi(phase);
    BIG_COMPLEX_FLOAT phase_complex(cos_v, sin_v);
    stateVector = phase_complex * stateVector;
}

long double CFLOBDDQuantumVerifier::GetProbability(std::map<unsigned int, int>& qubit_vals)
{
    auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
    std::string s(std::pow(2, tmp.root->level-1), 'X');
    for (unsigned int i = 0; i < numQubits; i++)
    {
        if (qubit_vals.find(i) != qubit_vals.end())
        {
            if (qubit_vals[i] == 0)
                s[i] = '0';
            else if (qubit_vals[i] == 1)
                s[i] = '1';   
        }
    }
    auto restricted = Matrix1234ComplexFloatBoost::MkRestrictMatrix(tmp.root->level, s);
    tmp = tmp * restricted;
    tmp.CountPaths();
    return VectorComplexFloatBoost::getNonZeroProbability(tmp);
}

std::string CFLOBDDQuantumVerifier::Measure() 
{
    auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
    tmp.CountPaths();
    return VectorComplexFloatBoost::Sampling(tmp, true).substr(0, numQubits); 
}

/// ****** BDDQuantumVerifier *******

#include <mpfr.h>
#define RND_TYPE MPFR_RNDF

BDDQuantumVerifier::BDDQuantumVerifier(unsigned int numQubits, int seed) : QuantumVerifier(numQubits, seed)
{
    mgr = new Cudd(0,0);

    if (numQubits > 512) // Based on experiments
    {
        mpfr_set_default_prec(300);
        CUDD_VALUE_TYPE epsilon;
        mpfr_init_set_si(epsilon.real, -1 * (200) , RND_TYPE);
        mpfr_exp10(epsilon.real, epsilon.real, RND_TYPE);
        mpfr_init_set_si(epsilon.imag, 0, RND_TYPE);
        mgr->SetEpsilon(epsilon);
    }

    for (unsigned int i = 0; i < numQubits; i++)
    {
        x_vars.push_back(mgr->addVar(2*i));
        y_vars.push_back(mgr->addVar(2*i + 1));
    }

    stateVector = mgr->addOne();
    // e_{0..0}
    for (unsigned int i = 0; i < numQubits; i++)
    {
        stateVector *= ~x_vars[i];
    }
}

BDDQuantumVerifier::BDDQuantumVerifier()
{
    mgr = new Cudd(0,0);
    numQubits = 0;
}

BDDQuantumVerifier::~BDDQuantumVerifier()
{
    // delete mgr;
}

void BDDQuantumVerifier::setNumQubits(unsigned int n)
{
    numQubits = n;
    x_vars.clear();
    y_vars.clear();
    for (unsigned int i = 0; i < numQubits; i++)
    {
        x_vars.push_back(mgr->addVar(2*i));
        y_vars.push_back(mgr->addVar(2*i + 1));
    }

    stateVector = mgr->addOne();
    // e_{0..0}
    for (unsigned int i = 0; i < numQubits; i++)
    {
        stateVector *= ~x_vars[i];
    }
}

void BDDQuantumVerifier::ApplyIdentityGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD IDGate = ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    stateVector = IDGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyHadamardGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    
    CUDD_VALUE_TYPE H;
    mpfr_init_set_d(H.real, 1, RND_TYPE);
    mpfr_init_set_d(H.imag, 0, RND_TYPE);
    mpfr_div_d(H.real, H.real, sqrt(2), RND_TYPE);
    ADD H_val = mgr->constant(H);
    mpfr_clear(H.real); mpfr_clear(H.imag);
    
    ADD HGate = (~y_vars[index] + y_vars[index] * (~x_vars[index] - x_vars[index])) * H_val;
    // HGate.print(2*numQubits, 2);
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    stateVector = HGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
    hadamard_count++;
}

void BDDQuantumVerifier::ApplyNOTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD XGate = ~y_vars[index] * x_vars[index] + y_vars[index] * ~x_vars[index];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    stateVector = XGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyPauliYGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    CUDD_VALUE_TYPE I;
    mpfr_init_set_d(I.real, 0, RND_TYPE);
    mpfr_init_set_d(I.imag, 1, RND_TYPE);
    ADD I_val = mgr->constant(I);
    mpfr_clear(I.real); mpfr_clear(I.imag);

    ADD Y =  (y_vars[index] * x_vars[index] - ~y_vars[index] * x_vars[index]) * I_val;
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    stateVector = Y.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyPauliZGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD ZGate = ~y_vars[index] * ~x_vars[index] - y_vars[index] * x_vars[index];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    stateVector = ZGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplySGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    CUDD_VALUE_TYPE I;
    mpfr_init_set_d(I.real, 0, RND_TYPE);
    mpfr_init_set_d(I.imag, 1, RND_TYPE);
    ADD I_val = mgr->constant(I);
    mpfr_clear(I.real); mpfr_clear(I.imag);

    ADD SGate = ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index] * I_val;
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    stateVector = SGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyCNOTGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD CNOTGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
                 + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
                 + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * x_vars[controlled]
                 + y_vars[controller] * y_vars[controlled] * x_vars[controller] * ~x_vars[controlled];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
    tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
    stateVector = CNOTGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyGlobalPhase(double phase)
{
   if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    CUDD_VALUE_TYPE phase_complex;
    mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
    mpfr_const_pi(phase_complex.real, RND_TYPE);
    mpfr_const_pi(phase_complex.imag, RND_TYPE);
    mpfr_mul_d(phase_complex.real, phase_complex.real, phase, RND_TYPE);
    mpfr_mul_d(phase_complex.imag, phase_complex.imag, phase, RND_TYPE);
    mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
    mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
    ADD phase_add = mgr->constant(phase_complex);
    mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);
    stateVector = phase_add * stateVector; 
}

void BDDQuantumVerifier::ApplySwapGate(long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD SwapGate = ~y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * ~x_vars[index2]
                 + ~y_vars[index1] * y_vars[index2] * x_vars[index1] * ~x_vars[index2]
                 + y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * x_vars[index2]
                 + y_vars[index1] * y_vars[index2] * x_vars[index1] * ~x_vars[index2];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
    tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
    stateVector = SwapGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyiSwapGate(long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }

    CUDD_VALUE_TYPE I;
    mpfr_init_set_d(I.real, 0, RND_TYPE);
    mpfr_init_set_d(I.imag, 1, RND_TYPE);
    ADD I_val = mgr->constant(I);
    mpfr_clear(I.real); mpfr_clear(I.imag);

    ADD iSwapGate = ~y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * ~x_vars[index2]
                 + ~y_vars[index1] * y_vars[index2] * x_vars[index1] * ~x_vars[index2] * I_val
                 + y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * x_vars[index2] * I_val
                 + y_vars[index1] * y_vars[index2] * x_vars[index1] * ~x_vars[index2];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
    tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
    stateVector = iSwapGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyCZGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD CZGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
                 + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
                 + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * ~x_vars[controlled]
                 - y_vars[controller] * y_vars[controlled] * x_vars[controller] * x_vars[controlled];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
    tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
    stateVector = CZGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyCPGate(long int controller, long int controlled, double theta)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }

    CUDD_VALUE_TYPE phase_complex;
    mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
    mpfr_const_pi(phase_complex.real, RND_TYPE);
    mpfr_const_pi(phase_complex.imag, RND_TYPE);
    mpfr_mul_d(phase_complex.real, phase_complex.real, theta, RND_TYPE);
    mpfr_mul_d(phase_complex.imag, phase_complex.imag, theta, RND_TYPE);
    mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
    mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
    ADD phase_add = mgr->constant(phase_complex);
    mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);


    ADD CPGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
                 + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
                 + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * x_vars[controlled]
                 + y_vars[controller] * y_vars[controlled] * x_vars[controller] * ~x_vars[controlled] * phase_add;
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
    tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
    stateVector = CPGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyPhaseShiftGate(unsigned int index, double theta)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    CUDD_VALUE_TYPE phase_complex;
    mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
    mpfr_const_pi(phase_complex.real, RND_TYPE);
    mpfr_const_pi(phase_complex.imag, RND_TYPE);
    mpfr_mul_d(phase_complex.real, phase_complex.real, theta, RND_TYPE);
    mpfr_mul_d(phase_complex.imag, phase_complex.imag, theta, RND_TYPE);
    mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
    mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
    ADD phase_add = mgr->constant(phase_complex);
    mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);

    ADD PhaseShiftGate =  ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index] * phase_add;
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    stateVector = PhaseShiftGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    CUDD_VALUE_TYPE phase_complex;
    mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
    mpfr_const_pi(phase_complex.real, RND_TYPE);
    mpfr_const_pi(phase_complex.imag, RND_TYPE);
    mpfr_mul_d(phase_complex.real, phase_complex.real, 0.25, RND_TYPE);
    mpfr_mul_d(phase_complex.imag, phase_complex.imag, 0.25, RND_TYPE);
    mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
    mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
    ADD phase_add = mgr->constant(phase_complex);
    mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);

    ADD TGate =  ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index] * phase_add;
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]); 
    stateVector = TGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyCSGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }

    CUDD_VALUE_TYPE phase_complex;
    mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
    mpfr_const_pi(phase_complex.real, RND_TYPE);
    mpfr_const_pi(phase_complex.imag, RND_TYPE);
    mpfr_mul_d(phase_complex.real, phase_complex.real, 0.5, RND_TYPE);
    mpfr_mul_d(phase_complex.imag, phase_complex.imag, 0.5, RND_TYPE);
    mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
    mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
    ADD phase_add = mgr->constant(phase_complex);
    mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);

    ADD CSGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
                 + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
                 + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * ~x_vars[controlled]
                 + y_vars[controller] * y_vars[controlled] * x_vars[controller] * x_vars[controlled] * phase_add;
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
    tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
    stateVector = CSGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyCCNOTGate(long int controller, long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    
    ADD CCNOTGate = ~y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
                  + ~y_vars[controller] * ~y_vars[index1] * y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * x_vars[index2]
                  + ~y_vars[controller] * y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * x_vars[index1] * ~x_vars[index2]
                  + ~y_vars[controller] * y_vars[index1] * y_vars[index2] * ~x_vars[controller] * x_vars[index1] * x_vars[index2]
                  + y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
                  + y_vars[controller] * ~y_vars[index1] * y_vars[index2] * x_vars[controller] * ~x_vars[index1] * x_vars[index2]
                  + y_vars[controller] * y_vars[index1] * ~y_vars[index2] * x_vars[controller] * x_vars[index1] * x_vars[index2]
                  + y_vars[controller] * y_vars[index1] * y_vars[index2] * x_vars[controller] * x_vars[index1] * ~x_vars[index2];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
    tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
    stateVector = CCNOTGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumVerifier::ApplyCSwapGate(long int controller, long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD CSwapGate = ~y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
                  + ~y_vars[controller] * ~y_vars[index1] * y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * x_vars[index2]
                  + ~y_vars[controller] * y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * x_vars[index1] * ~x_vars[index2]
                  + ~y_vars[controller] * y_vars[index1] * y_vars[index2] * ~x_vars[controller] * x_vars[index1] * x_vars[index2]
                  + y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
                  + y_vars[controller] * ~y_vars[index1] * y_vars[index2] * x_vars[controller] * x_vars[index1] * ~x_vars[index2]
                  + y_vars[controller] * y_vars[index1] * ~y_vars[index2] * x_vars[controller] * ~x_vars[index1] * x_vars[index2]
                  + y_vars[controller] * y_vars[index1] * y_vars[index2] * x_vars[controller] * x_vars[index1] * x_vars[index2];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
    tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
    stateVector = CSwapGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

long double BDDQuantumVerifier::GetProbability(std::map<unsigned int, int>& qubit_vals)
{
    // stateVector.print(2*numQubits, 2);
    ADD tmp = stateVector.SquareTerminalValues();
    ADD s_add = mgr->addOne();

    for (auto it = qubit_vals.begin(); it != qubit_vals.end(); it++)
    {
        if (it->second == 0)
            s_add *= ~x_vars[it->first];
        else
            s_add *= x_vars[it->first];
    }
    tmp = tmp * s_add;
    tmp.UpdatePathInfo(2, numQubits);
    // tmp.PrintPathInfo();
    return tmp.GetProbability(2, numQubits);
}

std::string BDDQuantumVerifier::Measure() 
{
    ADD tmp = stateVector.SquareTerminalValues();
    tmp.UpdatePathInfo(2, numQubits);
    return tmp.SamplePath(numQubits, 2, "").substr(0, numQubits); 
}








