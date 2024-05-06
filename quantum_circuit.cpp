#include "quantum_circuit.h"
#include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
#include "cflobdd/CFLOBDD/vector_complex_float_boost.h"
#include <random>

QuantumCircuit::QuantumCircuit(unsigned int numQubits, int seed) :  numQubits (numQubits) 
{
    mt.seed(seed);
    srand(seed);
    hadamard_count = 0;
}
QuantumCircuit::QuantumCircuit() :  numQubits (0), hadamard_count (0) {}
QuantumCircuit::~QuantumCircuit() {}

using namespace CFL_OBDD;


CFLOBDDQuantumCircuit::CFLOBDDQuantumCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
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

CFLOBDDQuantumCircuit::CFLOBDDQuantumCircuit()
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

CFLOBDDQuantumCircuit::~CFLOBDDQuantumCircuit()
{
    DisposeOfTripleProductCache();
	DisposeOfPairProductCache();
	CFLOBDDNodeHandle::DisposeOfReduceCache();
}

void CFLOBDDQuantumCircuit::setNumQubits(unsigned int num)
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

void CFLOBDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto H = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkIdRelationInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(H, stateVector);
}

void CFLOBDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
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

void CFLOBDDQuantumCircuit::ApplyNOTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto X = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkNegationMatrixInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(X, stateVector);
}

void CFLOBDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto Y = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPauliYMatrixInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(Y, stateVector);
}

void CFLOBDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto Z = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPauliZMatrixInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(Z, stateVector);
}

void CFLOBDDQuantumCircuit::ApplySGate(unsigned int index)
{
    auto S = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkSGateInterleaved);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector);
}

void CFLOBDDQuantumCircuit::ApplyCNOTGate(long int controller, long int controlled)
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

void CFLOBDDQuantumCircuit::ApplySwapGate(long int index1, long int index2)
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

void CFLOBDDQuantumCircuit::ApplyiSwapGate(long int index1, long int index2)
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

void CFLOBDDQuantumCircuit::ApplyCZGate(long int controller, long int controlled)
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

void CFLOBDDQuantumCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
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

void CFLOBDDQuantumCircuit::ApplyCSGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ApplyCPGate(controller, controlled, 0.5);
}

void CFLOBDDQuantumCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPhaseShiftGateInterleaved, theta);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector); 
}

void CFLOBDDQuantumCircuit::ApplyTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPhaseShiftGateInterleaved, 0.25);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector); 
}

void CFLOBDDQuantumCircuit::ApplyCCNOTGate(long int controller1, long int controller2, long int controlled)
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

void CFLOBDDQuantumCircuit::ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta)
{
    assert(controller1 != controlled);
    assert(controller2 != controlled);
    assert(controller1 != controller2);
    if (controller1 < controller2 && controller2 < controlled)
    {
        // a b c
        auto C = Matrix1234ComplexFloatBoost::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controller2, controlled, theta);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (controller1 < controlled && controlled < controller2)
    {
        // a c b   
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller2);
        auto C = Matrix1234ComplexFloatBoost::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controlled, controller2, theta);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (controller2 < controller1 && controller1 < controlled)
    {
        // b a c
        ApplyCCPGate(controller2, controller1, controlled, theta);
    }
    else if (controller2 < controlled && controlled < controller1)
    {
        // b c a
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller1);
        auto C = Matrix1234ComplexFloatBoost::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller2, controlled, controller1, theta);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector); 
    }
    else if (controlled < controller1 && controller1 < controller2)
    {
        // c a b
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller2);
        // b a c
        auto C = Matrix1234ComplexFloatBoost::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller1, controller2, theta);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }
    else if (controlled < controller2 && controller2 < controller1)
    {
        // c b a
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller1);
        // a b c
        auto C = Matrix1234ComplexFloatBoost::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller2, controller1, theta);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
    }

} 

void CFLOBDDQuantumCircuit::ApplyMCXGate(std::vector<long int> controllers, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }

    sort(controllers.begin(), controllers.end());
    // assumption controllers < controlled, TODO: change to a more generalized version

    auto C = Matrix1234ComplexFloatBoost::MkMCX(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controllers, controlled);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);

        
}

void CFLOBDDQuantumCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
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
        auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index1, controller, index2);
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

void CFLOBDDQuantumCircuit::ApplySXGate(unsigned int index)
{
    abort();
}

void CFLOBDDQuantumCircuit::ApplySYGate(unsigned int index)
{
    abort();
}



void CFLOBDDQuantumCircuit::ApplyGlobalPhase(double phase)
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

std::string getBits(unsigned int n, int len){
  std::string s = "";
  if (n == 0){
    for (int i = 0; i < len; i++)
      s += '0';
  }
  while (n != 0){
    int tmp = n % 2;
    s += (tmp == 0) ? '0' : '1';
    n = n/2;
  }
  while (s.length() < (unsigned int)len)
    s += '0';
  std::reverse(s.begin(), s.end());
  return s;
}

std::string getInterleavedBits(std::string s1, std::string s2){
  std::string s = "";
  for (unsigned int i = 0; i < s1.length(); i++)
  {
    s = s + s1[i] + s2[i];
  }
  return s;
}

std::string padBits(std::string s1, unsigned int num){
  std::string s = "";
  for (unsigned int i = 0; i < s1.length(); i++)
  {
    s = s + s1[i];
  }
  for (unsigned int i = s1.length(); i < num; i++)
  {
    s = s + '0';
  }
  return s;
}

void CFLOBDDQuantumCircuit::ApplyCUGate(long int a, long int N)
{
    unsigned int level = stateVector.root->level;
    CFLOBDD_COMPLEX_BIG cu_gate = VectorComplexFloatBoost::MkBasisVector(level, 0);

    for (unsigned int i = 1; i < pow(2, numQubits-1); i++)
	{
		std::string bit_s = getBits(i, numQubits);
        bit_s = getInterleavedBits(bit_s, bit_s);
        bit_s = padBits(bit_s, std::pow(2, level));
        cu_gate = cu_gate + VectorComplexFloatBoost::MkBasisVector(level, bit_s);
	}

	for (unsigned int i = 0; i < pow(2, numQubits-1); i++)
	{
		std::string bit_s1 = getBits(pow(2, numQubits-1) + i, numQubits);
		std::string bit_s2 = getBits(pow(2, numQubits-1) + (a * i)%N, numQubits);
        std::string bit_s = getInterleavedBits(bit_s1, bit_s2);
        bit_s = padBits(bit_s, std::pow(2, level));
        cu_gate = cu_gate + VectorComplexFloatBoost::MkBasisVector(level, bit_s);
	}

    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(cu_gate, stateVector);
}

long double CFLOBDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
{
    // stateVector.print(std::cout);
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

std::string CFLOBDDQuantumCircuit::Measure() 
{
    // std::cout << stateVector << std::endl;

    auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
    tmp.CountPaths();
    return VectorComplexFloatBoost::Sampling(tmp, true).substr(0, numQubits); 
}

std::string CFLOBDDQuantumCircuit::MeasureAndCollapse(std::vector<long int>& indices)
{
    std::string final_s = "";
    for (long int i = 0; i < indices.size(); i++)
    {
        auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
        tmp.CountPaths();
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        char x = VectorComplexFloatBoost::Sampling(tmp, true).substr(0, numQubits)[indices[i]]; 
        final_s = final_s + x;
        std::string s(std::pow(2, tmp.root->level-1), 'X');
        s[indices[i]] = x;
        auto restricted = Matrix1234ComplexFloatBoost::MkRestrictMatrix(tmp.root->level, s);
        tmp = tmp * restricted;
        stateVector = tmp;
    }
    // stateVector.print(std::cout);
    return final_s;
}

unsigned long long int CFLOBDDQuantumCircuit::GetPathCount(long double prob)
{
    auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
    tmp.CountPaths();
    return VectorComplexFloatBoost::GetPathCount(tmp, prob);  
}

unsigned int CFLOBDDQuantumCircuit::Size()
{
    unsigned int nodeCount = 0, edgeCount = 0, returnEdgeCount = 0, returnEdgeObjCount = 0;
    stateVector.CountNodesAndEdges(nodeCount, edgeCount, returnEdgeCount, returnEdgeObjCount);
    return (nodeCount + edgeCount);
}

CFLOBDD_COMPLEX_BIG CreateGateF(std::string indices, CFLOBDD_COMPLEX_BIG(*f)(unsigned int))
{
    if (indices.find('0') == std::string::npos)
    {
        unsigned int level = log2(indices.length() * 2);
        return f(level);
    }
    else if (indices.find('1') == std::string::npos)
    {
        unsigned int level = log2(indices.length() * 2);
        return Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level);
    }
    else
    {
        auto F1 = CreateGateF(indices.substr(0, indices.length()/2), f);
        auto F2 = CreateGateF(indices.substr(indices.length()/2), f); 
        return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(F1, F2);
    }
}

CFLOBDDQuantumGate* CFLOBDDQuantumCircuit::CreateIdentityGate(std::string indices)
{
    long int level = ceil(log2(indices.length()));
    auto I = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level);
    return new CFLOBDDQuantumGate(I);
}


CFLOBDDQuantumGate* CFLOBDDQuantumCircuit::CreateHadamardGate(std::string indices)
{
    long int adjusted_len = std::pow(2, ceil(log2(indices.length())));
    std::string new_indices(adjusted_len, '0');
    for (unsigned int i = 0; i < indices.length(); i++)
        new_indices[i] = indices[i];
    auto H = CreateGateF(new_indices, Matrix1234ComplexFloatBoost::MkWalshInterleaved);
    return new CFLOBDDQuantumGate(H);
}

CFLOBDDQuantumGate* CFLOBDDQuantumCircuit::CreateNOTGate(std::string indices)
{
    long int adjusted_len = std::pow(2, ceil(log2(indices.length())));
    std::string new_indices(adjusted_len, '0');
    for ( int i = 0; i < indices.length(); i++)
        new_indices[i] = indices[i];
    auto H = CreateGateF(new_indices, Matrix1234ComplexFloatBoost::MkNegationMatrixInterleaved);
    return new CFLOBDDQuantumGate(H);
}

CFLOBDDQuantumGate* CFLOBDDQuantumCircuit::CreateCNOTGate(long int controller, long int controlled)
{
    if (controller < controlled)
    {
        auto C = Matrix1234ComplexFloatBoost::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller, controlled);
        return new CFLOBDDQuantumGate(C);
    }
    else
    {
        auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = Matrix1234ComplexFloatBoost::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
        C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
        return new CFLOBDDQuantumGate(C); 
    }
}

void CFLOBDDQuantumCircuit::ApplyGate(CFLOBDDQuantumGate* m)
{
    auto c = m->GetGate();
    assert(c.root->level == stateVector.root->level);
    stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(c, stateVector); 
}



CFLOBDDQuantumGate* CFLOBDDQuantumCircuit::KroneckerProduct(CFLOBDDQuantumGate* m1, CFLOBDDQuantumGate* m2)
{
    auto c1 = m1->GetGate();
    auto c2 = m2->GetGate();
    assert(c1.root->level == c2.root->level);
    auto c = Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(c1, c2);
    return new CFLOBDDQuantumGate(c);
}

CFLOBDDQuantumGate* CFLOBDDQuantumCircuit::GateGateApply(CFLOBDDQuantumGate* m1, CFLOBDDQuantumGate* m2)
{
    auto c1 = m1->GetGate();
    auto c2 = m2->GetGate();
    assert(c1.root->level == c2.root->level);
    auto c = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(c1, c2);
    return new CFLOBDDQuantumGate(c);
}

CFLOBDDQuantumState* CFLOBDDQuantumCircuit::GetState()
{
    return new CFLOBDDQuantumState(stateVector);
}

/// ****** BDDQuantumCircuit *******

#include <mpfr.h>
#define RND_TYPE MPFR_RNDF

BDDQuantumCircuit::BDDQuantumCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
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

    if (numQubits > 2048) // Based on experiments
    {
        mpfr_set_default_prec(500);
        CUDD_VALUE_TYPE epsilon;
        mpfr_init_set_si(epsilon.real, -1 * (300) , RND_TYPE);
        mpfr_exp10(epsilon.real, epsilon.real, RND_TYPE);
        mpfr_init_set_si(epsilon.imag, 0, RND_TYPE);
        mgr->SetEpsilon(epsilon);
    }

    for (unsigned int i = 0; i < numQubits; i++)
    {
        x_vars.push_back(mgr->addVar(3*i));
        y_vars.push_back(mgr->addVar(3*i + 1));
        z_vars.push_back(mgr->addVar(3*i + 2));
    }

    stateVector = mgr->addOne();
    // e_{0..0}
    for (unsigned int i = 0; i < numQubits; i++)
    {
        stateVector *= ~x_vars[i];
    }
}

BDDQuantumCircuit::BDDQuantumCircuit()
{
    mgr = new Cudd(0,0);
    numQubits = 0;
}

BDDQuantumCircuit::~BDDQuantumCircuit()
{
    // delete mgr;
}

void BDDQuantumCircuit::setNumQubits(unsigned int n)
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

void BDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
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

void BDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
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
    // if (index == numQubits - 1)
    //     stateVector.print(2 * numQubits, 2);
    hadamard_count++;
}

void BDDQuantumCircuit::ApplyNOTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD XGate = ~y_vars[index] * x_vars[index] + y_vars[index] * ~x_vars[index];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
    // std::cout << "index: " << index << std::endl;
    // stateVector.print(2 * numQubits, 2);
    stateVector = XGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
    // std::cout << "after" << std::endl;
    // stateVector.print(2 * numQubits, 2);
}

void BDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
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

void BDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
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

void BDDQuantumCircuit::ApplySGate(unsigned int index)
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

void BDDQuantumCircuit::ApplyCNOTGate(long int controller, long int controlled)
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

void BDDQuantumCircuit::ApplyGlobalPhase(double phase)
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

void BDDQuantumCircuit::ApplySwapGate(long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ADD SwapGate = ~y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * ~x_vars[index2]
                 + ~y_vars[index1] * y_vars[index2] * x_vars[index1] * ~x_vars[index2]
                 + y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * x_vars[index2]
                 + y_vars[index1] * y_vars[index2] * x_vars[index1] * x_vars[index2];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
    tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
    stateVector = SwapGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumCircuit::ApplyiSwapGate(long int index1, long int index2)
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
                 + y_vars[index1] * y_vars[index2] * x_vars[index1] * x_vars[index2];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
    tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
    stateVector = iSwapGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

void BDDQuantumCircuit::ApplyCZGate(long int controller, long int controlled)
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

void BDDQuantumCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
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

void BDDQuantumCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
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

void BDDQuantumCircuit::ApplyTGate(unsigned int index)
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

void BDDQuantumCircuit::ApplyCSGate(long int controller, long int controlled)
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

void BDDQuantumCircuit::ApplyCCNOTGate(long int controller, long int index1, long int index2)
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
    // ADD CCNOTGate = y_vars[controller] * y_vars[index1] * ~y_vars[index2] * x_vars[controller] * x_vars[index1] * x_vars[index2]
    //               + y_vars[controller] * y_vars[index1] * y_vars[index2] * x_vars[controller] * x_vars[index1] * ~x_vars[index2];
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
    tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
    stateVector = CCNOTGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
}

std::string intToBinaryString(unsigned int N, int len)
{
    std::string s(len, '0');
    int i = len-1;
    while (N > 0)
    {
        int c = N % 2;
        N = N/2;
        s[i] = (c == 1) ? '1' : '0';
        i--;
    }
    return s;
}

void BDDQuantumCircuit::ApplyMCXGate(std::vector<long int> controllers, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }

     ADD MCXGate = mgr->addZero();
     sort(controllers.begin(), controllers.end());
     unsigned int numControllers = controllers.size();
     for (unsigned int i = 0; i < std::pow(2, numControllers); i++)
     {
        std::string perm = intToBinaryString(i, numControllers);
        if (perm.find('0') == std::string::npos)
        {
            ADD tmp = mgr->addOne();
            for (unsigned int j = 0; j < numControllers; j++)
            {
                tmp *= y_vars[controllers[j]] * x_vars[controllers[j]];   
            }
            MCXGate += tmp * ~y_vars[controlled] * x_vars[controlled] + tmp * y_vars[controlled] * ~x_vars[controlled];   
        }
        else
        {
            ADD tmp = mgr->addOne();
            for (unsigned int j = 0; j < numControllers; j++)
            {
                if (perm[j] == '0')
                {
                    tmp *= ~y_vars[controllers[j]] * ~x_vars[controllers[j]];
                }
                else
                {
                    tmp *= y_vars[controllers[j]] * x_vars[controllers[j]];   
                }
            }
            MCXGate += tmp * ~y_vars[controlled] * ~x_vars[controlled] + tmp * y_vars[controlled] * x_vars[controlled];
        }
     }

    std::vector<ADD> tmp_x, tmp_y; 
    for (unsigned int i = 0; i < controllers.size(); i++)
    {
        tmp_x.push_back(x_vars[controllers[i]]);    
        tmp_y.push_back(y_vars[controllers[i]]);
    }
    tmp_x.push_back(x_vars[controlled]);
    tmp_y.push_back(y_vars[controlled]);
    stateVector = MCXGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);

}

void BDDQuantumCircuit::ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta)
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
    
    ADD CCPGate = ~y_vars[controller1] * ~y_vars[controller2] * ~y_vars[controlled] * ~x_vars[controller1] * ~x_vars[controller2] * ~x_vars[controlled]
                  + ~y_vars[controller1] * ~y_vars[controller2] * y_vars[controlled] * ~x_vars[controller1] * ~x_vars[controller2] * x_vars[controlled]
                  + ~y_vars[controller1] * y_vars[controller2] * ~y_vars[controlled] * ~x_vars[controller1] * x_vars[controller2] * ~x_vars[controlled]
                  + ~y_vars[controller1] * y_vars[controller2] * y_vars[controlled] * ~x_vars[controller1] * x_vars[controller2] * x_vars[controlled]
                  + y_vars[controller1] * ~y_vars[controller2] * ~y_vars[controlled] * x_vars[controller1] * ~x_vars[controller2] * ~x_vars[controlled]
                  + y_vars[controller1] * ~y_vars[controller2] * y_vars[controlled] * x_vars[controller1] * ~x_vars[controller2] * x_vars[controlled]
                  + y_vars[controller1] * y_vars[controller2] * ~y_vars[controlled] * x_vars[controller1] * x_vars[controller2] * ~x_vars[controlled] * phase_add
                  + y_vars[controller1] * y_vars[controller2] * y_vars[controlled] * x_vars[controller1] * x_vars[controller2] * x_vars[controlled] * phase_add;
    std::vector<ADD> tmp_x, tmp_y; 
    tmp_x.push_back(x_vars[controller1]); tmp_x.push_back(x_vars[controller2]); tmp_x.push_back(x_vars[controlled]);
    tmp_y.push_back(y_vars[controller1]); tmp_y.push_back(y_vars[controller2]); tmp_y.push_back(y_vars[controlled]);
    stateVector = CCPGate.MatrixMultiply(stateVector, tmp_x);
    stateVector = stateVector.SwapVariables(tmp_y, tmp_x);

}

void BDDQuantumCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
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

void BDDQuantumCircuit::ApplySXGate(unsigned int index)
{
    abort();
}

void BDDQuantumCircuit::ApplySYGate(unsigned int index)
{
    abort();
}



void BDDQuantumCircuit::ApplyCUGate(long int a, long int N)
{
    ADD cu_gate = mgr->addZero();

	for (unsigned int i = 0; i < pow(2, numQubits-1); i++)
	{
		ADD tmp_cu_gate = mgr->addOne();
		std::string bit_s = getBits(i, numQubits);
		for (unsigned int j = 0; j < numQubits; j++)
		{
            // std::cout << i << " " << j << std::endl;
			if (bit_s[j] == '0')
				tmp_cu_gate = tmp_cu_gate * ~x_vars[j] * ~y_vars[j];
			else if (bit_s[j] == '1')
				tmp_cu_gate = tmp_cu_gate * x_vars[j] * y_vars[j];
		}
		cu_gate = cu_gate + tmp_cu_gate;
	}

	for (unsigned int i = 0; i < pow(2, numQubits-1); i++)
	{
		ADD tmp_cu_gate = mgr->addOne();
		std::string bit_s1 = getBits(pow(2, numQubits-1) + i, numQubits);
		std::string bit_s2 = getBits(pow(2, numQubits-1) + (a * i)%N, numQubits);
		for (unsigned int j = 0; j < numQubits; j++)
		{
			if (bit_s1[j] == '0')
				tmp_cu_gate = tmp_cu_gate * ~y_vars[j];
			else if (bit_s1[j] == '1')
				tmp_cu_gate = tmp_cu_gate * y_vars[j];

			if (bit_s2[j] == '0')
				tmp_cu_gate = tmp_cu_gate * ~x_vars[j];
			else if (bit_s2[j] == '1')
				tmp_cu_gate = tmp_cu_gate * x_vars[j];
		}
		cu_gate = cu_gate + tmp_cu_gate;
	}

    stateVector = cu_gate.MatrixMultiply(stateVector, x_vars);
    stateVector = stateVector.SwapVariables(y_vars, x_vars);

}

long double BDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
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

std::string BDDQuantumCircuit::Measure() 
{
    // stateVector.print(numQubits, 2);
    ADD tmp = stateVector.SquareTerminalValues();
    tmp.UpdatePathInfo(3, numQubits);
    return tmp.SamplePath(numQubits, 3, "").substr(0, numQubits); 
}

std::string BDDQuantumCircuit::MeasureAndCollapse(std::vector<long int>& indices)
{
    std::string final_s = "";
    for (long int i = 0; i < indices.size(); i++)
    {
        auto tmp = stateVector.SquareTerminalValues();
        tmp.UpdatePathInfo(2, numQubits);
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        char x = tmp.SamplePath(numQubits, 2, "").substr(0, numQubits)[indices[i]]; 
        final_s = final_s + x;
        ADD s_add = mgr->addOne();
        if (x == '0')
            s_add *= ~x_vars[0];
        else
            s_add *= x_vars[0];
        tmp = tmp * s_add;
        stateVector = tmp;
    }
    // stateVector.print(std::cout);
    return final_s;
}

unsigned long long int BDDQuantumCircuit::GetPathCount(long double prob)
{
    ADD tmp = stateVector.SquareTerminalValues();
    tmp.UpdatePathInfo(3, numQubits);
    return tmp.GetPathCount(numQubits, 3, prob); 
}

unsigned int BDDQuantumCircuit::Size()
{
    return stateVector.nodeCount();
}


BDDQuantumGate* BDDQuantumCircuit::CreateHadamardGate(std::string indices)
{
    ADD gate = mgr->addOne();

    CUDD_VALUE_TYPE H;
    mpfr_init_set_d(H.real, 1, RND_TYPE);
    mpfr_init_set_d(H.imag, 0, RND_TYPE);
    mpfr_div_d(H.real, H.real, sqrt(2), RND_TYPE);
    ADD H_val = mgr->constant(H);
    mpfr_clear(H.real); mpfr_clear(H.imag);

    for (unsigned int i = 0; i < indices.length(); i++)
    {
        if (indices[i] == '1')
        {
            ADD HGate = (~x_vars[i] + x_vars[i] * (~y_vars[i] - y_vars[i])) * H_val; 
            gate = gate * HGate;
        }
        else
        {
            ADD IDGate = ~x_vars[i] * ~y_vars[i] + x_vars[i] * y_vars[i];
            gate = gate * IDGate; 
        }
    }
    return new BDDQuantumGate(gate);
}

BDDQuantumGate* BDDQuantumCircuit::CreateIdentityGate(std::string indices)
{
    ADD gate = mgr->addOne();

    for (unsigned int i = 0; i < numQubits; i++)
    {
        ADD IDGate = ~x_vars[i] * ~y_vars[i] + x_vars[i] * y_vars[i];
        gate = gate * IDGate; 
    }
    return new BDDQuantumGate(gate);
}

BDDQuantumGate* BDDQuantumCircuit::CreateNOTGate(std::string indices)
{
    ADD gate = mgr->addOne();

    for (unsigned int i = 0; i < indices.length(); i++)
    {
        if (indices[i] == '1')
        {

            ADD XGate = ~x_vars[i] * y_vars[i] + x_vars[i] * ~y_vars[i];
            gate = gate * XGate;
        }
        else
        {
            ADD IDGate = ~x_vars[i] * ~y_vars[i] + x_vars[i] * y_vars[i];
            gate = gate * IDGate; 
        }
    }
    return new BDDQuantumGate(gate);
}

BDDQuantumGate* BDDQuantumCircuit::CreateCNOTGate(long int controller, long int controlled)
{
    ADD CNOTGate = ~x_vars[controller] * ~x_vars[controlled] * ~y_vars[controller] * ~y_vars[controlled]
                 + ~x_vars[controller] * x_vars[controlled] * ~y_vars[controller] * y_vars[controlled]
                 + x_vars[controller] * ~x_vars[controlled] * y_vars[controller] * y_vars[controlled]
                 + x_vars[controller] * x_vars[controlled] * y_vars[controller] * ~y_vars[controlled];
    
    for (unsigned int i = 0; i < numQubits; i++)
    {
        if (i != controller && i != controlled)
        {
            ADD IDGate = ~x_vars[i] * ~y_vars[i] + x_vars[i] * y_vars[i];
            CNOTGate = CNOTGate * IDGate; 
        }
    }
    return new BDDQuantumGate(CNOTGate);
}

BDDQuantumGate* BDDQuantumCircuit::KroneckerProduct(BDDQuantumGate* m1, BDDQuantumGate* m2)
{
    auto c1 = m1->GetGate();
    auto c2 = m2->GetGate();
    return new BDDQuantumGate(c1 * c2);
}

BDDQuantumGate* BDDQuantumCircuit::GateGateApply(BDDQuantumGate* m1, BDDQuantumGate* m2)
{
    ADD c1 = m1->GetGate();
    ADD c2 = m2->GetGate();
    c1 = c1.SwapVariables(y_vars, z_vars);
    c2 = c2.SwapVariables(x_vars, z_vars);
    ADD c = c1.MatrixMultiply(c2, z_vars);
    return new BDDQuantumGate(c);
}

void BDDQuantumCircuit::ApplyGate(BDDQuantumGate* m)
{
    auto c = m->GetGate();
    std::vector<ADD> tmp_y, tmp_x;
    tmp_y.push_back(y_vars[0]); tmp_y.push_back(y_vars[2]);
    tmp_x.push_back(x_vars[0]);  tmp_x.push_back(x_vars[2]);
    stateVector = stateVector.SwapVariables(x_vars, y_vars);
    stateVector = c.MatrixMultiply(stateVector, y_vars);
}

BDDQuantumState* BDDQuantumCircuit::GetState()
{
    return new BDDQuantumState(stateVector);
}

// *******************
// Weighted BDD
// *******************

#include "cflobdd/CFLOBDD/wvector_complex_fb_mul.h"
#include "cflobdd/CFLOBDD/weighted_cross_product.h"
#include "cflobdd/CFLOBDD/weighted_cross_product_bdd.h"

WeightedBDDQuantumCircuit::WeightedBDDQuantumCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
{
    // Initialize
    WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable_Ann();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitIdentityNodeTable();	
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitReduceCache();
	WeightedMatrix1234ComplexFloatBoostMul::Matrix1234Initializer();
	WeightedVectorComplexFloatBoostMul::VectorInitializer();
	InitWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();

    WeightedBDDNodeHandle<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitLeafNodes();
	InitWeightedBDDPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
    //
    std::string s(2 * numQubits, '0');
    stateVector = WeightedVectorComplexFloatBoostMul::MkBasisVector(2 * numQubits, s, 0);
}

WeightedBDDQuantumCircuit::WeightedBDDQuantumCircuit()
{
    // Initialize
    WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable_Ann();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitIdentityNodeTable();	
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitReduceCache();
	WeightedMatrix1234ComplexFloatBoostMul::Matrix1234Initializer();
	WeightedVectorComplexFloatBoostMul::VectorInitializer();
	InitWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();

    WeightedBDDNodeHandle<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitLeafNodes();
	InitWeightedBDDPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
    numQubits = 0;
    //
}

WeightedBDDQuantumCircuit::~WeightedBDDQuantumCircuit()
{
    DisposeOfWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
    DisposeOfWeightedBDDPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
}

void WeightedBDDQuantumCircuit::setNumQubits(unsigned int num)
{
    numQubits = num;
    stateVector = WeightedVectorComplexFloatBoostMul::MkBasisVector(numQubits, 0, 0);
}

WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateF_WBDD(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, int, unsigned int))
{
    WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = f(2, 0, 2*i);
    if (i == 0)
    {
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0, 2);
        return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I, false); 
    }
    else if (i == n - 1)
    {
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0);
        return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I, H, false);
    }
    else {
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I1 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * i, 0);
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I2 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n - i - 1), 0, 2*(i + 1));
        auto T = WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I2, false);
        return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I1, T, false);
    }
}

WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateFWithParam_WBDD(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, double, int, unsigned int), double theta)
{
    WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = f(2, theta, 0, 2*i);
    if (i == 0)
    {
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0, 2);
        return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I, false); 
    }
    else if (i == n - 1)
    {
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0);
        return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I, H, false);
    }
    else {
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I1 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * i, 0);
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I2 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n - i - 1), 0, 2*(i + 1));
        auto T = WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I2, false);
        return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I1, T, false);
    }
}


void WeightedBDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto H = ApplyGateF_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved);
    // auto H = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(numQubits, 0);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(H, stateVector);
}

void WeightedBDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto H = ApplyGateF_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkWalshInterleaved);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(H, stateVector);
    // stateVector.print(std::cout);
    // std::cout << std::endl;
    hadamard_count++;
}

void WeightedBDDQuantumCircuit::ApplyNOTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    // stateVector.print(std::cout);
    auto X = ApplyGateF_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkNegationMatrixInterleaved);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(X, stateVector);
    // stateVector.print(std::cout);
}

void WeightedBDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto Y = ApplyGateF_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliYGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Y, stateVector);
}

void WeightedBDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto Z = ApplyGateF_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliZGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Z, stateVector);
}

void WeightedBDDQuantumCircuit::ApplySGate(unsigned int index)
{
    auto S = ApplyGateF_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkSGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector);
}

void WeightedBDDQuantumCircuit::ApplyCNOTGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(2 * numQubits, numQubits, controller, controlled, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
        // stateVector.print(std::cout);
    }
    else
    {
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller, 0);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(2 * numQubits, numQubits, controlled, controller, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedBDDQuantumCircuit::ApplySwapGate(long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(index1 != index2);
    
    if (index1 < index2)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index1, index2, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index2, index1, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedBDDQuantumCircuit::ApplyiSwapGate(long int index1, long int index2)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(index1 != index2);
    
    if (index1 < index2)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkiSwapGate(2 * numQubits, index1, index2, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkiSwapGate(2 * numQubits, index2, index1, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedBDDQuantumCircuit::ApplyCZGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controller, controlled, 1.0, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller, 0);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controlled, controller, 1.0, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedBDDQuantumCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controller, controlled, theta, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller, 0);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controlled, controller, theta, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    } 
}

void WeightedBDDQuantumCircuit::ApplyCSGate(long int controller, long int controlled)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    ApplyCPGate(controller, controlled, 0.5);
}

void WeightedBDDQuantumCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto S = ApplyGateFWithParam_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, theta);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector); 
}

void WeightedBDDQuantumCircuit::ApplyTGate(unsigned int index)
{
    if (checkForInit(numQubits) == false)
    {
        std::cout << "Number of Qubits is unset" << std::endl;
        abort();   
    }
    auto S = ApplyGateFWithParam_WBDD(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, 0.25);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector); 
}

void WeightedBDDQuantumCircuit::ApplyCCNOTGate(long int controller1, long int controller2, long int controlled)
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
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controller1, controller2, controlled, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller1 < controlled && controlled < controller2)
    {
        // a c b   
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller2, 0);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controller1, controlled, controller2, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller2 < controller1 && controller1 < controlled)
    {
        // b a c
        ApplyCCNOTGate(controller2, controller1, controlled);
    }
    else if (controller2 < controlled && controlled < controller1)
    {
        // b c a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller1, 0);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controller2, controlled, controller1, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector); 
    }
    else if (controlled < controller1 && controller1 < controller2)
    {
        // c a b
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller2, 0);
        // b a c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controlled, controller1, controller2, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controlled < controller2 && controller2 < controller1)
    {
        // c b a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller1, 0);
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controlled, controller2, controller1, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedBDDQuantumCircuit::ApplyMCXGate(std::vector<long int> controllers, long int controlled)
{

}

void WeightedBDDQuantumCircuit::ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta)
{
    
}

void WeightedBDDQuantumCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
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
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, controller, index1, index2, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller < index2 && index2 < index1)
    {
        // a c b   
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, controller, index2, index1, 0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index1 < controller && controller < index2)
    {
        // b a c
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index1, controller, 0);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index1, controller, index2, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index1 < index2 && index2 < controller)
    {
        // b c a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index1, controller, 0);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index1, index2, controller, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index2 < controller && controller < index1)
    {
        // c a b
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index2, controller, 0);
        // b a c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index2, controller, index1, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index2 < index1 && index1 < controller)
    {
        // c b a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index2, controller, 0);
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index2, index1, controller, 0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedBDDQuantumCircuit::ApplySXGate(unsigned int index)
{
    abort();
}

void WeightedBDDQuantumCircuit::ApplySYGate(unsigned int index)
{
    abort();
}

void WeightedBDDQuantumCircuit::ApplyCUGate(long int a, long int N)
{
    abort();
}

void WeightedBDDQuantumCircuit::ApplyGlobalPhase(double phase)
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

long double WeightedBDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
{
    auto tmp = stateVector;
    std::string s(numQubits, 'X');
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
    auto restricted = WeightedMatrix1234ComplexFloatBoostMul::MkRestrictMatrix(2 * numQubits, s, 0);
    tmp = tmp * restricted;
    tmp.ComputeWeightOfPathsAsAmpsToExits();
    return WeightedVectorComplexFloatBoostMul::getNonZeroProbability(tmp);
}

std::string WeightedBDDQuantumCircuit::Measure() 
{
    auto tmp = stateVector;
    // tmp.print(std::cout);
    tmp.ComputeWeightOfPathsAsAmpsToExits();
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return WeightedVectorComplexFloatBoostMul::Sampling(tmp, true, this->mt, dis).substr(0, numQubits); 
}

std::string WeightedBDDQuantumCircuit::MeasureAndCollapse(std::vector<long int>& indices)
{
    return "";
}

unsigned long long int WeightedBDDQuantumCircuit::GetPathCount(long double prob)
{
    std::cout << "Error! Operation not supported in WBDDs" << std::endl;
    abort();
} 

unsigned int WeightedBDDQuantumCircuit::Size()
{
    unsigned int nodeCount = 0, edgeCount = 0, returnEdgeCount = 0, returnEdgeObjCount = 0;
    stateVector.CountNodesAndEdges(nodeCount, edgeCount, returnEdgeCount, returnEdgeObjCount);
    return (nodeCount + edgeCount);
}


WeightedBDDQuantumGate* WeightedBDDQuantumCircuit::CreateHadamardGate(std::string indices)
{
    abort();
}

WeightedBDDQuantumGate* WeightedBDDQuantumCircuit::CreateIdentityGate(std::string indices)
{
    abort();
}

WeightedBDDQuantumGate* WeightedBDDQuantumCircuit::CreateNOTGate(std::string indices)
{
    abort();
}

WeightedBDDQuantumGate* WeightedBDDQuantumCircuit::KroneckerProduct(WeightedBDDQuantumGate* m1, WeightedBDDQuantumGate* m2)
{
    abort();
}

WeightedBDDQuantumGate* WeightedBDDQuantumCircuit::CreateCNOTGate(long int controller, long int controlled)
{
    abort();
}

WeightedBDDQuantumGate* WeightedBDDQuantumCircuit::GateGateApply(WeightedBDDQuantumGate* m1, WeightedBDDQuantumGate* m2)
{
    abort();
}

void WeightedBDDQuantumCircuit::ApplyGate(WeightedBDDQuantumGate* m)
{
    abort();
}

WeightedBDDQuantumState* WeightedBDDQuantumCircuit::GetState()
{
    return new WeightedBDDQuantumState(stateVector);
}

#include "cflobdd/CFLOBDD/wvector_complex_fb_mul.h"
#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"
#include "cflobdd/CFLOBDD/weighted_cross_product.h"

WeightedCFLOBDDQuantumCircuit::WeightedCFLOBDDQuantumCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
{
    // Initialize
    WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable_Ann();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitIdentityNodeTable();	
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitReduceCache();
	WeightedMatrix1234ComplexFloatBoostMul::Matrix1234Initializer();
	WeightedVectorComplexFloatBoostMul::VectorInitializer();
	InitWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
    //
    unsigned int level = ceil(log2(numQubits)) + 1;
    std::string s(std::pow(2, level), '0');
    stateVector = WeightedVectorComplexFloatBoostMul::MkBasisVector(level, s);
}

WeightedCFLOBDDQuantumCircuit::WeightedCFLOBDDQuantumCircuit()
{
    // Initialize
    WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable_Ann();
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitIdentityNodeTable();	
	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitReduceCache();
	WeightedMatrix1234ComplexFloatBoostMul::Matrix1234Initializer();
	WeightedVectorComplexFloatBoostMul::VectorInitializer();
	InitWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();

    numQubits = 0;
    //
}

WeightedCFLOBDDQuantumCircuit::~WeightedCFLOBDDQuantumCircuit()
{
    DisposeOfWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
}

void WeightedCFLOBDDQuantumCircuit::setNumQubits(unsigned int num)
{
    numQubits = num;
    stateVector = WeightedVectorComplexFloatBoostMul::MkBasisVector(numQubits, 0);
}

WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateF_WCFLOBDD(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, int, unsigned int))
{
    if (n == 1)
    {
        return f(1, 1, 0);
    }
    else {
        int level = ceil(log2(n/2));
        if (i < n/2)
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = ApplyGateF_WCFLOBDD(n/2, i, f);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, T);
        }
        else
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(T, ApplyGateF_WCFLOBDD(n/2, i - n/2, f)); 
        }
    }
}

WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateF_WCFLOBDD_2(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int))
{
    if (n == 1)
    {
        return f(1);
    }
    else {
        int level = ceil(log2(n/2));
        if (i < n/2)
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = ApplyGateF_WCFLOBDD_2(n/2, i, f);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, T);
        }
        else
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(T, ApplyGateF_WCFLOBDD_2(n/2, i - n/2, f)); 
        }
    }
}

WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateFWithParam_WCFLOBDD(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, double, int, unsigned int), double theta)
{
    if (n == 1)
    {
        return f(1, theta, 1, 0);
    }
    else {
        int level = ceil(log2(n/2));
        if (i < n/2)
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = ApplyGateFWithParam_WCFLOBDD(n/2, i, f, theta);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, T);
        }
        else
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(T, ApplyGateFWithParam_WCFLOBDD(n/2, i - n/2, f, theta)); 
        }
    }
}


void WeightedCFLOBDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
{
    auto H = ApplyGateF_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(H, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
{
    auto H = ApplyGateF_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkWalshInterleaved);
    // H.print(std::cout);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(H, stateVector);
    // std::cout << std::endl;
    hadamard_count++;
}

void WeightedCFLOBDDQuantumCircuit::ApplyCNOTGate(long int controller, long int controlled)
{
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level-1), controller, controlled);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

std::string WeightedCFLOBDDQuantumCircuit::Measure() 
{
    auto tmp = stateVector;
    // stateVector.print(std::cout);
    // WeightedVectorComplexFloatBoostMul::PrintVector(stateVector, std::cout, numQubits);
    // return "";
    tmp.ComputeWeightOfPathsAsAmpsToExits();
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return WeightedVectorComplexFloatBoostMul::Sampling(tmp, true, this->mt, dis).substr(0, numQubits); 
}

std::string WeightedCFLOBDDQuantumCircuit::MeasureAndCollapse(std::vector<long int>& indices)
{
    std::string final_s = "";
    for (long int i = 0; i < indices.size(); i++)
    {
        auto tmp = stateVector;
        tmp.ComputeWeightOfPathsAsAmpsToExits();
        std::uniform_real_distribution<double> dis(0.0, 1.0);
        char x = WeightedVectorComplexFloatBoostMul::Sampling(tmp, true, this->mt, dis).substr(0, numQubits)[indices[i]]; 
        final_s = final_s + x;
        std::string s(std::pow(2, tmp.root->level-1), 'X');
        s[indices[i]] = x;
        auto restricted = WeightedMatrix1234ComplexFloatBoostMul::MkRestrictMatrix(tmp.root->level, s);
        tmp = tmp * restricted;
        tmp.ComputeWeightOfPathsAsAmpsToExits();
        if (tmp.root->rootConnection.returnMapHandle[0] == 0 && tmp.root->rootConnection.returnMapHandle.Size() == 1)
        {
            tmp.root->rootConnection.factor = 0;
        }
        else if (tmp.root->rootConnection.returnMapHandle[0] == 0 && tmp.root->rootConnection.returnMapHandle.Size() == 2)
        {
            tmp.root->rootConnection.factor = 1.0/sqrt(tmp.root->rootConnection.entryPointHandle->handleContents->numWeightsOfPathsAsAmpsToExit[1]);
        }
        else if (tmp.root->rootConnection.returnMapHandle.Size() == 1 && tmp.root->rootConnection.returnMapHandle[0] == 1)
        {
            tmp.root->rootConnection.factor = 1.0/sqrt(tmp.root->rootConnection.entryPointHandle->handleContents->numWeightsOfPathsAsAmpsToExit[0]); 
        }
        else if (tmp.root->rootConnection.returnMapHandle.Size() == 2 && tmp.root->rootConnection.returnMapHandle[0] == 1)
        {
            tmp.root->rootConnection.factor = 1.0/sqrt(tmp.root->rootConnection.entryPointHandle->handleContents->numWeightsOfPathsAsAmpsToExit[0]); 
        }
        stateVector = tmp;
    }
    // stateVector.print(std::cout);
    return final_s;
}

void WeightedCFLOBDDQuantumCircuit::ApplyNOTGate(unsigned int index)
{
    auto X = ApplyGateF_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkNegationMatrixInterleaved);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(X, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
{
    auto Y = ApplyGateF_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliYGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Y, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
{
    auto Z = ApplyGateF_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliZGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Z, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplySXGate(unsigned int index)
{
    auto SX = ApplyGateF_WCFLOBDD_2(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkSXGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(SX, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplySYGate(unsigned int index)
{
    auto SY = ApplyGateF_WCFLOBDD_2(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkSYGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(SY, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta)
{
    assert(controller1 != controlled);
    assert(controller2 != controlled);
    assert(controller1 != controller2);
    if (controller1 < controller2 && controller2 < controlled)
    {
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controller2, controlled, theta);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller1 < controlled && controlled < controller2)
    {
        // a c b   
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller2);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controlled, controller2, theta);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller2 < controller1 && controller1 < controlled)
    {
        // b a c
        ApplyCCPGate(controller2, controller1, controlled, theta);
    }
    else if (controller2 < controlled && controlled < controller1)
    {
        // b c a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller1);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller2, controlled, controller1, theta);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector); 
    }
    else if (controlled < controller1 && controller1 < controller2)
    {
        // c a b
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller2);
        // b a c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller1, controller2, theta);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controlled < controller2 && controller2 < controller1)
    {
        // c b a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller1);
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCP(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller2, controller1, theta);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }

} 

void WeightedCFLOBDDQuantumCircuit::ApplySGate(unsigned int index)
{
    auto S = ApplyGateF_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkSGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector);
}


void WeightedCFLOBDDQuantumCircuit::ApplySwapGate(long int index1, long int index2)
{
    assert(index1 != index2);
    
    if (index1 < index2)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, index1, index2);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, index2, index1);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedCFLOBDDQuantumCircuit::ApplyiSwapGate(long int index1, long int index2)
{
    assert(index1 != index2);
    
    if (index1 < index2)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkiSwapGate(stateVector.root->level, index1, index2);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkiSwapGate(stateVector.root->level, index2, index1);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedCFLOBDDQuantumCircuit::ApplyCZGate(long int controller, long int controlled)
{
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(stateVector.root->level, controller, controlled, 1.0);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(stateVector.root->level, controlled, controller, 1.0);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedCFLOBDDQuantumCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
{
    assert(controller != controlled);
    
    if (controller < controlled)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(stateVector.root->level, controller, controlled, theta);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else
    {
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(stateVector.root->level, controlled, controller, theta);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    } 
}

void WeightedCFLOBDDQuantumCircuit::ApplyCSGate(long int controller, long int controlled)
{
    ApplyCPGate(controller, controlled, 0.5);
}

void WeightedCFLOBDDQuantumCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
{
    auto S = ApplyGateFWithParam_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, theta);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyTGate(unsigned int index)
{
    auto S = ApplyGateFWithParam_WCFLOBDD(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, 0.25);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector); 
}

void WeightedCFLOBDDQuantumCircuit::ApplyCCNOTGate(long int controller1, long int controller2, long int controlled)
{
    assert(controller1 != controlled);
    assert(controller2 != controlled);
    assert(controller1 != controller2);
    if (controller1 < controller2 && controller2 < controlled)
    {
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controller2, controlled);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller1 < controlled && controlled < controller2)
    {
        // a c b   
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller2);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controlled, controller2);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller2 < controller1 && controller1 < controlled)
    {
        // b a c
        ApplyCCNOTGate(controller2, controller1, controlled);
    }
    else if (controller2 < controlled && controlled < controller1)
    {
        // b c a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller1);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller2, controlled, controller1);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector); 
    }
    else if (controlled < controller1 && controller1 < controller2)
    {
        // c a b
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller2);
        // b a c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller1, controller2);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controlled < controller2 && controller2 < controller1)
    {
        // c b a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller1);
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller2, controller1);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedCFLOBDDQuantumCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
{
    assert(controller != index1);
    assert(controller != index2);
    assert(index1 != index2);
    
    if (controller < index1 && index1 < index2)
    {
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate2(stateVector.root->level, controller, index1, index2);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (controller < index2 && index2 < index1)
    {
        // a c b   
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate2(stateVector.root->level, controller, index2, index1);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index1 < controller && controller < index2)
    {
        // b a c
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, index1, controller);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate2(stateVector.root->level, index1, controller, index2);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index1 < index2 && index2 < controller)
    {
        // b c a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, index1, controller);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate2(stateVector.root->level, index1, index2, controller);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index2 < controller && controller < index1)
    {
        // c a b
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, index2, controller);
        // b a c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate2(stateVector.root->level, index2, controller, index1);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
    else if (index2 < index1 && index1 < controller)
    {
        // c b a
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, index2, controller);
        // a b c
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate2(stateVector.root->level, index2, index1, controller);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
    }
}

void WeightedCFLOBDDQuantumCircuit::ApplyGlobalPhase(double phase)
{
    auto cos_v = boost::math::cos_pi(phase);
    auto sin_v = boost::math::sin_pi(phase);
    BIG_COMPLEX_FLOAT phase_complex(cos_v, sin_v);
    stateVector = phase_complex * stateVector;
}

void WeightedCFLOBDDQuantumCircuit::ApplyMCXGate(std::vector<long int> controllers, long int controlled)
{

}

long double WeightedCFLOBDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
{
}

unsigned int WeightedCFLOBDDQuantumCircuit::Size()
{
    unsigned int nodeCount = 0, edgeCount = 0, returnEdgeCount = 0, returnEdgeObjCount = 0;
    stateVector.CountNodesAndEdges(nodeCount, edgeCount, returnEdgeCount, returnEdgeObjCount);
    return (nodeCount + edgeCount);
}

unsigned long long int WeightedCFLOBDDQuantumCircuit::GetPathCount(long double prob)
{
    std::cout << "Error! Operation not supported in WBDDs" << std::endl;
    abort();
} 


WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL CreateGateF_WCFLOBDD(std::string indices, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, int, unsigned int))
{
    if (indices.find('0') == std::string::npos)
    {
        unsigned int level = log2(indices.length() * 2);
        return f(level, 1, 0);
    }
    else if (indices.find('1') == std::string::npos)
    {
        unsigned int level = log2(indices.length() * 2);
        return WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level);;
    }
    else
    {
        auto F1 = CreateGateF_WCFLOBDD(indices.substr(0, indices.length()/2), f);
        auto F2 = CreateGateF_WCFLOBDD(indices.substr(indices.length()/2), f); 
        return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(F1, F2);
    }
}


WeightedCFLOBDDQuantumGate* WeightedCFLOBDDQuantumCircuit::CreateHadamardGate(std::string indices)
{
    long int adjusted_len = std::pow(2, ceil(log2(indices.length())));
    std::string new_indices(adjusted_len, '0');
    for (unsigned int i = 0; i < indices.length(); i++)
        new_indices[i] = indices[i];
    auto H = CreateGateF_WCFLOBDD(new_indices, WeightedMatrix1234ComplexFloatBoostMul::MkWalshInterleaved);
    return new WeightedCFLOBDDQuantumGate(H);
}

WeightedCFLOBDDQuantumGate* WeightedCFLOBDDQuantumCircuit::CreateIdentityGate(std::string indices)
{
    long int level = ceil(log2(indices.length()));
    auto I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level);
    return new WeightedCFLOBDDQuantumGate(I);
}

WeightedCFLOBDDQuantumGate* WeightedCFLOBDDQuantumCircuit::CreateNOTGate(std::string indices)
{
    long int adjusted_len = std::pow(2, ceil(log2(indices.length())));
    std::string new_indices(adjusted_len, '0');
    for (unsigned int i = 0; i < indices.length(); i++)
        new_indices[i] = indices[i];
    auto H = CreateGateF_WCFLOBDD(new_indices, WeightedMatrix1234ComplexFloatBoostMul::MkNegationMatrixInterleaved);
    return new WeightedCFLOBDDQuantumGate(H);
}

WeightedCFLOBDDQuantumGate* WeightedCFLOBDDQuantumCircuit::KroneckerProduct(WeightedCFLOBDDQuantumGate* m1, WeightedCFLOBDDQuantumGate* m2)
{
    auto c1 = m1->GetGate();
    auto c2 = m2->GetGate();
    assert(c1.root->level == c2.root->level);
    auto c = WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(c1, c2);
    return new WeightedCFLOBDDQuantumGate(c);
}

WeightedCFLOBDDQuantumGate* WeightedCFLOBDDQuantumCircuit::CreateCNOTGate(long int controller, long int controlled)
{
    if (controller < controlled)
    {
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller, controlled);
        return new WeightedCFLOBDDQuantumGate(C);
    }
    else
    {
        auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(stateVector.root->level, controlled, controller);
        auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
        C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
        return new WeightedCFLOBDDQuantumGate(C); 
    }
}

WeightedCFLOBDDQuantumGate* WeightedCFLOBDDQuantumCircuit::GateGateApply(WeightedCFLOBDDQuantumGate* m1, WeightedCFLOBDDQuantumGate* m2)
{
    auto c1 = m1->GetGate();
    auto c2 = m2->GetGate();
    assert(c1.root->level == c2.root->level);
    auto c = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(c1, c2);
    return new WeightedCFLOBDDQuantumGate(c);
}

void WeightedCFLOBDDQuantumCircuit::ApplyGate(WeightedCFLOBDDQuantumGate* m)
{
    auto c = m->GetGate();
    assert(c.root->level == stateVector.root->level);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(c, stateVector); 
}

WeightedCFLOBDDQuantumState* WeightedCFLOBDDQuantumCircuit::GetState()
{
    return new WeightedCFLOBDDQuantumState(stateVector);
}





//// ****** MQTDD *******

MQTDDCircuit::MQTDDCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
{
    // Initialize
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numQubits);
    stateVector = ddp->makeZeroState(numQubits);
}

MQTDDCircuit::MQTDDCircuit()
{
    // Initialize
    numQubits = 0;
    //
}

MQTDDCircuit::~MQTDDCircuit()
{
}

void MQTDDCircuit::setNumQubits(unsigned int num)
{
    numQubits = num;
    ddp = std::make_unique<dd::Package<DDPackageConfig>>(numQubits);
    stateVector = ddp->makeZeroState(numQubits);
}

void MQTDDCircuit::ApplyIdentityGate(unsigned int index)
{
    auto i_op = ddp->makeGateDD(dd::Imat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(i_op, stateVector);
}

void MQTDDCircuit::ApplyHadamardGate(unsigned int index)
{
    auto h_op = ddp->makeGateDD(dd::Hmat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(h_op, stateVector);
}

void MQTDDCircuit::ApplyCNOTGate(long int controller, long int controlled)
{
    Control c;
    c.qubit = numQubits - 1 - controller;
    auto cnot_op = ddp->makeGateDD(dd::Xmat, numQubits, c, numQubits - 1 - controlled);
    stateVector = ddp->multiply(cnot_op, stateVector);
}

std::string MQTDDCircuit::Measure()
{
    std::string s =  ddp->measureAll(stateVector, false, mt);
    return s;
}

unsigned int MQTDDCircuit::Size()
{
    return ddp->size(stateVector);
}

std::string MQTDDCircuit::MeasureAndCollapse(std::vector<long int>& indices)
{
    // ddp->printVector(stateVector);
    std::string s = "";
    for (long int i = 0; i < indices.size(); i++)
    {
        ddp->incRef(stateVector);
        char x = ddp->measureOneCollapsing(stateVector, numQubits - 1 - indices[i], false, mt);
        s += x;
    }
    return s;
}

void MQTDDCircuit::ApplyNOTGate(unsigned int index)
{
    auto x_op = ddp->makeGateDD(dd::Xmat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(x_op, stateVector);
}

void MQTDDCircuit::ApplyPauliYGate(unsigned int index)
{
    auto y_op = ddp->makeGateDD(dd::Ymat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(y_op, stateVector);
}

void MQTDDCircuit::ApplyPauliZGate(unsigned int index)
{
    auto z_op = ddp->makeGateDD(dd::Zmat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(z_op, stateVector);
}

void MQTDDCircuit::ApplySXGate(unsigned int index)
{
    auto sx_op = ddp->makeGateDD(dd::SXmat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(sx_op, stateVector);
}

void MQTDDCircuit::ApplySYGate(unsigned int index)
{
    auto sy_op = ddp->makeGateDD(dd::SYmat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(sy_op, stateVector);
}

void MQTDDCircuit::ApplySGate(unsigned int index)
{
    auto s_op = ddp->makeGateDD(dd::Smat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(s_op, stateVector);
}


void MQTDDCircuit::ApplySwapGate(long int index1, long int index2)
{
    auto swap_op = ddp->makeSWAPDD(numQubits, Controls{}, numQubits - 1 - index1, numQubits - 1 - index2);
    stateVector = ddp->multiply(swap_op, stateVector);
}

void MQTDDCircuit::ApplyiSwapGate(long int index1, long int index2)
{
    auto swap_op = ddp->makeiSWAPDD(numQubits, Controls{}, numQubits - 1 - index1, numQubits - 1 - index2);
    stateVector = ddp->multiply(swap_op, stateVector);
}

void MQTDDCircuit::ApplyCZGate(long int controller, long int controlled)
{
    Control c;
    c.qubit = numQubits - 1 - controller;
    auto cz_op = ddp->makeGateDD(dd::Zmat, numQubits, c, numQubits - 1 - controlled);
    stateVector = ddp->multiply(cz_op, stateVector); 
}

void MQTDDCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
{
    Control c;
    c.qubit = numQubits - 1 - controller;
    auto cp_op = ddp->makeGateDD(dd::Phasemat(M_PI * theta), numQubits, c, numQubits - 1 - controlled);
    stateVector = ddp->multiply(cp_op, stateVector);
}

void MQTDDCircuit::ApplyCSGate(long int controller, long int controlled)
{
    Control c;
    c.qubit = numQubits - 1 - controller;
    auto cs_op = ddp->makeGateDD(dd::Smat, numQubits, c, numQubits - 1 - controlled);
    stateVector = ddp->multiply(cs_op, stateVector); 
}

void MQTDDCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
{
    auto s_op = ddp->makeGateDD(dd::Phasemat(M_PI * theta), numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(s_op, stateVector);
}

void MQTDDCircuit::ApplyTGate(unsigned int index)
{
    auto t_op = ddp->makeGateDD(dd::Tmat, numQubits, numQubits - 1 - index);
    stateVector = ddp->multiply(t_op, stateVector);
}

void MQTDDCircuit::ApplyCCNOTGate(long int controller1, long int controller2, long int controlled)
{
    Control c1, c2;
    c1.qubit = numQubits - 1 - controller1;
    c2.qubit = numQubits - 1 - controller2;
    auto ccnot_op = ddp->makeGateDD(dd::Xmat, numQubits, Controls{c1, c2}, numQubits - 1 - controlled);
    stateVector = ddp->multiply(ccnot_op, stateVector);
}

void MQTDDCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
{
    Control c;
    c.qubit = numQubits - 1 - controller;
    auto cswap_op = ddp->makeSWAPDD(numQubits, Controls{c}, numQubits - 1 - index1, numQubits - 1 - index2);
    stateVector = ddp->multiply(cswap_op, stateVector);
}

void MQTDDCircuit::ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta)
{
    Control c1, c2;
    c1.qubit = numQubits - 1 - controller1;
    c2.qubit = numQubits - 1 - controller2;
    auto ccp_op = ddp->makeGateDD(dd::Phasemat(M_PI * theta), numQubits, Controls{c1, c2}, numQubits - 1 - controlled);
    stateVector = ddp->multiply(ccp_op, stateVector);
}

void MQTDDCircuit::ApplyMCXGate(std::vector<long int> controllers, long int controlled)
{
    Controls c;
    for (unsigned int i = 0; i < controllers.size(); i++)
    {
        Control ct;
        ct.qubit = numQubits - 1 - controllers[i];
        c.insert(ct);
    }
    auto mcx_op = ddp->makeGateDD(dd::Xmat, numQubits, c, numQubits - 1 - controlled);
    stateVector = ddp->multiply(mcx_op, stateVector);
}

void MQTDDCircuit::ApplyGlobalPhase(double phase)
{
}

long double MQTDDCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
{
}

unsigned long long int MQTDDCircuit::GetPathCount(long double prob)
{
    std::cout << "Error! Operation not supported in WBDDs" << std::endl;
    abort();
}

mEdge CreateGateF(std::string indices, std::unique_ptr<Package<DDPackageConfig>>& ddp, const GateMatrix& mat)
{
    if (indices.find('1') == std::string::npos)
    {
        return ddp->makeIdent(indices.length());       
    }
    else if (indices.length() == 1)
    {
        return ddp->makeGateDD(mat, 1, 0);
    }
    else
    {
        auto F1 = CreateGateF(indices.substr(0, indices.length()/2), ddp, mat);
        auto F2 = CreateGateF(indices.substr(indices.length()/2), ddp, mat); 
        return ddp->kronecker(F1, F2, true);
    }
}


MQTDDQuantumGate* MQTDDCircuit::CreateHadamardGate(std::string indices)
{
    mEdge m = CreateGateF(indices, ddp, dd::Hmat);
    // ddp->printMatrix(m);
    return new MQTDDQuantumGate(m);//, ddp);
}

MQTDDQuantumGate* MQTDDCircuit::CreateIdentityGate(std::string indices)
{
    mEdge m = ddp->makeIdent(indices.length());
    return new MQTDDQuantumGate(m);//, ddp);
}

MQTDDQuantumGate* MQTDDCircuit::CreateNOTGate(std::string indices)
{
    mEdge m = CreateGateF(indices, ddp, dd::Xmat);
    return new MQTDDQuantumGate(m);//, ddp);
}

MQTDDQuantumGate* MQTDDCircuit::CreateCNOTGate(long int controller, long int controlled)
{
    Control c;
    c.qubit = numQubits - 1 - controller;
    auto cnot_op = ddp->makeGateDD(dd::Xmat, numQubits, c, numQubits - 1 - controlled);
    return new MQTDDQuantumGate(cnot_op);//, ddp); 
}

MQTDDQuantumGate* MQTDDCircuit::KroneckerProduct(MQTDDQuantumGate* m1, MQTDDQuantumGate* m2)
{
    auto c1 = m1->GetGate();
    auto c2 = m2->GetGate();
    auto c = ddp->kronecker(c1, c2, true);
    return new MQTDDQuantumGate(c);//, ddp);
}

MQTDDQuantumGate* MQTDDCircuit::GateGateApply(MQTDDQuantumGate* m1, MQTDDQuantumGate* m2)
{
    auto c1 = m1->GetGate();
    auto c2 = m2->GetGate();
    auto c = ddp->multiply(c1, c2);
    return new MQTDDQuantumGate(c);//, ddp);
}

void MQTDDCircuit::ApplyGate(MQTDDQuantumGate* m)
{
    auto c = m->GetGate();
    // ddp->printMatrix(c);
    stateVector = ddp->multiply(c, stateVector);
    // ddp->printVector(stateVector);
}

MQTDDQuantumState* MQTDDCircuit::GetState()
{
    return new MQTDDQuantumState(stateVector);//, ddp);
}