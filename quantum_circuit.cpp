#include "quantum_circuit.h"
// #include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
// #include "cflobdd/CFLOBDD/vector_complex_float_boost.h"
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

// CFLOBDDQuantumCircuit::CFLOBDDQuantumCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
// {
//     // Initialize
//         CFLOBDDNodeHandle::InitNoDistinctionTable();
//         CFLOBDDNodeHandle::InitAdditionInterleavedTable();
//         CFLOBDDNodeHandle::InitReduceCache();
//         InitPairProductCache();
//         InitTripleProductCache();
//         Matrix1234ComplexFloatBoost::Matrix1234Initializer();
//         VectorComplexFloatBoost::VectorInitializer();
//     //
//     unsigned int level = ceil(log2(numQubits)) + 1;
//     stateVector = VectorComplexFloatBoost::MkBasisVector(level, 0);
// }

// CFLOBDDQuantumCircuit::CFLOBDDQuantumCircuit()
// {
//     // Initialize
//         CFLOBDDNodeHandle::InitNoDistinctionTable();
//         CFLOBDDNodeHandle::InitAdditionInterleavedTable();
//         CFLOBDDNodeHandle::InitReduceCache();
//         InitPairProductCache();
//         InitTripleProductCache();
//         Matrix1234ComplexFloatBoost::Matrix1234Initializer();
//         VectorComplexFloatBoost::VectorInitializer();
//         numQubits = 0;
//     //
// }

// CFLOBDDQuantumCircuit::~CFLOBDDQuantumCircuit()
// {
//     DisposeOfTripleProductCache();
// 	DisposeOfPairProductCache();
// 	CFLOBDDNodeHandle::DisposeOfReduceCache();
// }

// void CFLOBDDQuantumCircuit::setNumQubits(unsigned int num)
// {
//     numQubits = num;
//     unsigned int level = ceil(log2(numQubits)) + 1;
//     stateVector = VectorComplexFloatBoost::MkBasisVector(level, 0); 
// }

// CFLOBDD_COMPLEX_BIG ApplyGateF(unsigned int n, unsigned int i, CFLOBDD_COMPLEX_BIG(*f)(unsigned int))
// {
//     if (n == 1)
//     {
//         return f(1);
//     }
//     else {
//         int level = ceil(log2(n/2));
//         if (i < n/2)
//         {
//             CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
//             CFLOBDD_COMPLEX_BIG H = ApplyGateF(n/2, i, f);
//             return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(H, T);
//         }
//         else
//         {
//             CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
//             return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(T, ApplyGateF(n/2, i - n/2, f)); 
//         }
//     }
// }

// CFLOBDD_COMPLEX_BIG ApplyGateFWithParam(unsigned int n, unsigned int i, CFLOBDD_COMPLEX_BIG(*f)(unsigned int, double), double theta)
// {
//     if (n == 1)
//     {
//         return f(1, theta);
//     }
//     else {
//         int level = ceil(log2(n/2));
//         if (i < n/2)
//         {
//             CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
//             CFLOBDD_COMPLEX_BIG H = ApplyGateFWithParam(n/2, i, f, theta);
//             return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(H, T);
//         }
//         else
//         {
//             CFLOBDD_COMPLEX_BIG T = Matrix1234ComplexFloatBoost::MkIdRelationInterleaved(level + 1);
//             return Matrix1234ComplexFloatBoost::KroneckerProduct2Vocs(T, ApplyGateFWithParam(n/2, i - n/2, f, theta)); 
//         }
//     }
// }

// bool checkForInit(unsigned int numQubits)
// {
//     return numQubits != 0;
// }

// void CFLOBDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto H = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkIdRelationInterleaved);
//     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(H, stateVector);
// }

// void CFLOBDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto H = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkWalshInterleaved);
//     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(H, stateVector);
//     hadamard_count++;
// }

// void CFLOBDDQuantumCircuit::ApplyNOTGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto X = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkNegationMatrixInterleaved);
//     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(X, stateVector);
// }

// void CFLOBDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     auto Y = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPauliYMatrixInterleaved);
// //     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(Y, stateVector);
// }

// void CFLOBDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     auto Z = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPauliZMatrixInterleaved);
// //     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(Z, stateVector);
// }

// void CFLOBDDQuantumCircuit::ApplySGate(unsigned int index)
// {
// //     auto S = ApplyGateF(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkSGateInterleaved);
// //     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector);
// }

// void CFLOBDDQuantumCircuit::ApplyCNOTGate(long int controller, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(controller != controlled);
    
//     if (controller < controlled)
//     {
//         auto C = Matrix1234ComplexFloatBoost::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller, controlled);
//         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
//     }
//     else
//     {
//         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller);
//         auto C = Matrix1234ComplexFloatBoost::MkCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller);
//         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
//         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
//         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
//     }
// }

// void CFLOBDDQuantumCircuit::ApplySwapGate(long int index1, long int index2)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     assert(index1 != index2);
    
// //     if (index1 < index2)
// //     {
// //         auto C = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index1, index2);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else
// //     {
// //         auto C = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index2, index1);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// }

// void CFLOBDDQuantumCircuit::ApplyiSwapGate(long int index1, long int index2)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     assert(index1 != index2);
    
// //     if (index1 < index2)
// //     {
// //         auto C = Matrix1234ComplexFloatBoost::MkiSwapGate(stateVector.root->level, index1, index2);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else
// //     {
// //         auto C = Matrix1234ComplexFloatBoost::MkiSwapGate(stateVector.root->level, index2, index1);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// }

// void CFLOBDDQuantumCircuit::ApplyCZGate(long int controller, long int controlled)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     assert(controller != controlled);
    
// //     if (controller < controlled)
// //     {
// //         auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controller, controlled, 1.0);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else
// //     {
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller);
// //         auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controlled, controller, 1.0);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// }

// void CFLOBDDQuantumCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     assert(controller != controlled);
    
// //     if (controller < controlled)
// //     {
// //         auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controller, controlled, theta);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else
// //     {
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller);
// //         auto C = Matrix1234ComplexFloatBoost::MkCPGate(stateVector.root->level, controlled, controller, theta);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     } 
// }

// void CFLOBDDQuantumCircuit::ApplyCSGate(long int controller, long int controlled)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     ApplyCPGate(controller, controlled, 0.5);
// }

// void CFLOBDDQuantumCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPhaseShiftGateInterleaved, theta);
// //     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector); 
// }

// void CFLOBDDQuantumCircuit::ApplyTGate(unsigned int index)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, Matrix1234ComplexFloatBoost::MkPhaseShiftGateInterleaved, 0.25);
// //     stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, stateVector); 
// }

// void CFLOBDDQuantumCircuit::ApplyCCNOTGate(long int controller1, long int controller2, long int controlled)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     assert(controller1 != controlled);
// //     assert(controller2 != controlled);
// //     assert(controller1 != controller2);
// //     if (controller1 < controller2 && controller2 < controlled)
// //     {
// //         // a b c
// //         auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controller2, controlled);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (controller1 < controlled && controlled < controller2)
// //     {
// //         // a c b   
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller2);
// //         auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller1, controlled, controller2);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (controller2 < controller1 && controller1 < controlled)
// //     {
// //         // b a c
// //         ApplyCCNOTGate(controller2, controller1, controlled);
// //     }
// //     else if (controller2 < controlled && controlled < controller1)
// //     {
// //         // b c a
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller1);
// //         auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controller2, controlled, controller1);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector); 
// //     }
// //     else if (controlled < controller1 && controller1 < controller2)
// //     {
// //         // c a b
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller2);
// //         // b a c
// //         auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller1, controller2);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (controlled < controller2 && controller2 < controller1)
// //     {
// //         // c b a
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, controlled, controller1);
// //         // a b c
// //         auto C = Matrix1234ComplexFloatBoost::MkCCNOT(stateVector.root->level, std::pow(2, stateVector.root->level - 1), controlled, controller2, controller1);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// }

// void CFLOBDDQuantumCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     assert(controller != index1);
// //     assert(controller != index2);
// //     assert(index1 != index2);
    
// //     if (controller < index1 && index1 < index2)
// //     {
// //         // a b c
// //         auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, controller, index1, index2);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (controller < index2 && index2 < index1)
// //     {
// //         // a c b   
// //         auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, controller, index2, index1);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (index1 < controller && controller < index2)
// //     {
// //         // b a c
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index1, controller);
// //         auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index1, controller, index2);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (index1 < index2 && index2 < controller)
// //     {
// //         // b c a
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index1, controller);
// //         auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index1, index2, controller);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (index2 < controller && controller < index1)
// //     {
// //         // c a b
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index2, controller);
// //         // b a c
// //         auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index2, controller, index1);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// //     else if (index2 < index1 && index1 < controller)
// //     {
// //         // c b a
// //         auto S = Matrix1234ComplexFloatBoost::MkSwapGate(stateVector.root->level, index2, controller);
// //         // a b c
// //         auto C = Matrix1234ComplexFloatBoost::MkCSwapGate(stateVector.root->level, index2, index1, controller);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, S);
// //         C = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(S, C);
// //         stateVector = Matrix1234ComplexFloatBoost::MatrixMultiplyV4WithInfo(C, stateVector);
// //     }
// }

// void CFLOBDDQuantumCircuit::ApplyGlobalPhase(double phase)
// {
// //     if (checkForInit(numQubits) == false)
// //     {
// //         std::cout << "Number of Qubits is unset" << std::endl;
// //         abort();   
// //     }
// //     auto cos_v = boost::math::cos_pi(phase);
// //     auto sin_v = boost::math::sin_pi(phase);
// //     BIG_COMPLEX_FLOAT phase_complex(cos_v, sin_v);
// //     stateVector = phase_complex * stateVector;
// }

// long double CFLOBDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
// {
// //     // stateVector.print(std::cout);
// //     auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
// //     std::string s(std::pow(2, tmp.root->level-1), 'X');
// //     for (unsigned int i = 0; i < numQubits; i++)
// //     {
// //         if (qubit_vals.find(i) != qubit_vals.end())
// //         {
// //             if (qubit_vals[i] == 0)
// //                 s[i] = '0';
// //             else if (qubit_vals[i] == 1)
// //                 s[i] = '1';   
// //         }
// //     }
// //     auto restricted = Matrix1234ComplexFloatBoost::MkRestrictMatrix(tmp.root->level, s);
// //     tmp = tmp * restricted;
// //     tmp.CountPaths();
// //     return VectorComplexFloatBoost::getNonZeroProbability(tmp);
// }

// std::string CFLOBDDQuantumCircuit::Measure() 
// {
//     auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
//     tmp.CountPaths();
//     return VectorComplexFloatBoost::Sampling(tmp, true).substr(0, numQubits); 
// }

// unsigned long long int CFLOBDDQuantumCircuit::GetPathCount(long double prob)
// {
// //     auto tmp = VectorComplexFloatBoost::VectorWithAmplitude(stateVector);
// //     tmp.CountPaths();
// //     return VectorComplexFloatBoost::GetPathCount(tmp, prob);  
// }

// /// ****** BDDQuantumCircuit *******

// #include <mpfr.h>
// #define RND_TYPE MPFR_RNDF

// BDDQuantumCircuit::BDDQuantumCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
// {
//     mgr = new Cudd(0,0);

//     if (numQubits > 512) // Based on experiments
//     {
//         mpfr_set_default_prec(300);
//         CUDD_VALUE_TYPE epsilon;
//         mpfr_init_set_si(epsilon.real, -1 * (200) , RND_TYPE);
//         mpfr_exp10(epsilon.real, epsilon.real, RND_TYPE);
//         mpfr_init_set_si(epsilon.imag, 0, RND_TYPE);
//         mgr->SetEpsilon(epsilon);
//     }

//     if (numQubits > 2048) // Based on experiments
//     {
//         mpfr_set_default_prec(500);
//         CUDD_VALUE_TYPE epsilon;
//         mpfr_init_set_si(epsilon.real, -1 * (300) , RND_TYPE);
//         mpfr_exp10(epsilon.real, epsilon.real, RND_TYPE);
//         mpfr_init_set_si(epsilon.imag, 0, RND_TYPE);
//         mgr->SetEpsilon(epsilon);
//     }

//     for (unsigned int i = 0; i < numQubits; i++)
//     {
//         x_vars.push_back(mgr->addVar(2*i));
//         y_vars.push_back(mgr->addVar(2*i + 1));
//     }

//     stateVector = mgr->addOne();
//     // e_{0..0}
//     for (unsigned int i = 0; i < numQubits; i++)
//     {
//         stateVector *= ~x_vars[i];
//     }
// }

// BDDQuantumCircuit::BDDQuantumCircuit()
// {
//     mgr = new Cudd(0,0);
//     numQubits = 0;
// }

// BDDQuantumCircuit::~BDDQuantumCircuit()
// {
//     // delete mgr;
// }

// void BDDQuantumCircuit::setNumQubits(unsigned int n)
// {
//     numQubits = n;
//     x_vars.clear();
//     y_vars.clear();
//     for (unsigned int i = 0; i < numQubits; i++)
//     {
//         x_vars.push_back(mgr->addVar(2*i));
//         y_vars.push_back(mgr->addVar(2*i + 1));
//     }

//     stateVector = mgr->addOne();
//     // e_{0..0}
//     for (unsigned int i = 0; i < numQubits; i++)
//     {
//         stateVector *= ~x_vars[i];
//     }
// }

// void BDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ADD IDGate = ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
//     stateVector = IDGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
    
//     CUDD_VALUE_TYPE H;
//     mpfr_init_set_d(H.real, 1, RND_TYPE);
//     mpfr_init_set_d(H.imag, 0, RND_TYPE);
//     mpfr_div_d(H.real, H.real, sqrt(2), RND_TYPE);
//     ADD H_val = mgr->constant(H);
//     mpfr_clear(H.real); mpfr_clear(H.imag);
    
//     ADD HGate = (~y_vars[index] + y_vars[index] * (~x_vars[index] - x_vars[index])) * H_val;
//     // HGate.print(2*numQubits, 2);
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
//     stateVector = HGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
//     // if (index == numQubits - 1)
//     //     stateVector.print(2 * numQubits, 2);
//     hadamard_count++;
// }

// void BDDQuantumCircuit::ApplyNOTGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ADD XGate = ~y_vars[index] * x_vars[index] + y_vars[index] * ~x_vars[index];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
//     // std::cout << "index: " << index << std::endl;
//     // stateVector.print(2 * numQubits, 2);
//     stateVector = XGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
//     // std::cout << "after" << std::endl;
//     // stateVector.print(2 * numQubits, 2);
// }

// void BDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     CUDD_VALUE_TYPE I;
//     mpfr_init_set_d(I.real, 0, RND_TYPE);
//     mpfr_init_set_d(I.imag, 1, RND_TYPE);
//     ADD I_val = mgr->constant(I);
//     mpfr_clear(I.real); mpfr_clear(I.imag);

//     ADD Y =  (y_vars[index] * x_vars[index] - ~y_vars[index] * x_vars[index]) * I_val;
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
//     stateVector = Y.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ADD ZGate = ~y_vars[index] * ~x_vars[index] - y_vars[index] * x_vars[index];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
//     stateVector = ZGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplySGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     CUDD_VALUE_TYPE I;
//     mpfr_init_set_d(I.real, 0, RND_TYPE);
//     mpfr_init_set_d(I.imag, 1, RND_TYPE);
//     ADD I_val = mgr->constant(I);
//     mpfr_clear(I.real); mpfr_clear(I.imag);

//     ADD SGate = ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index] * I_val;
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
//     stateVector = SGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyCNOTGate(long int controller, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ADD CNOTGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
//                  + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
//                  + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * x_vars[controlled]
//                  + y_vars[controller] * y_vars[controlled] * x_vars[controller] * ~x_vars[controlled];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
//     tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
//     stateVector = CNOTGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyGlobalPhase(double phase)
// {
//    if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     CUDD_VALUE_TYPE phase_complex;
//     mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
//     mpfr_const_pi(phase_complex.real, RND_TYPE);
//     mpfr_const_pi(phase_complex.imag, RND_TYPE);
//     mpfr_mul_d(phase_complex.real, phase_complex.real, phase, RND_TYPE);
//     mpfr_mul_d(phase_complex.imag, phase_complex.imag, phase, RND_TYPE);
//     mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
//     mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
//     ADD phase_add = mgr->constant(phase_complex);
//     mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);
//     stateVector = phase_add * stateVector; 
// }

// void BDDQuantumCircuit::ApplySwapGate(long int index1, long int index2)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ADD SwapGate = ~y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * ~x_vars[index2]
//                  + ~y_vars[index1] * y_vars[index2] * x_vars[index1] * ~x_vars[index2]
//                  + y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * x_vars[index2]
//                  + y_vars[index1] * y_vars[index2] * x_vars[index1] * x_vars[index2];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
//     tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
//     stateVector = SwapGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyiSwapGate(long int index1, long int index2)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }

//     CUDD_VALUE_TYPE I;
//     mpfr_init_set_d(I.real, 0, RND_TYPE);
//     mpfr_init_set_d(I.imag, 1, RND_TYPE);
//     ADD I_val = mgr->constant(I);
//     mpfr_clear(I.real); mpfr_clear(I.imag);

//     ADD iSwapGate = ~y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * ~x_vars[index2]
//                  + ~y_vars[index1] * y_vars[index2] * x_vars[index1] * ~x_vars[index2] * I_val
//                  + y_vars[index1] * ~y_vars[index2] * ~x_vars[index1] * x_vars[index2] * I_val
//                  + y_vars[index1] * y_vars[index2] * x_vars[index1] * x_vars[index2];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
//     tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
//     stateVector = iSwapGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyCZGate(long int controller, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ADD CZGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
//                  + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
//                  + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * ~x_vars[controlled]
//                  - y_vars[controller] * y_vars[controlled] * x_vars[controller] * x_vars[controlled];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
//     tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
//     stateVector = CZGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }

//     CUDD_VALUE_TYPE phase_complex;
//     mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
//     mpfr_const_pi(phase_complex.real, RND_TYPE);
//     mpfr_const_pi(phase_complex.imag, RND_TYPE);
//     mpfr_mul_d(phase_complex.real, phase_complex.real, theta, RND_TYPE);
//     mpfr_mul_d(phase_complex.imag, phase_complex.imag, theta, RND_TYPE);
//     mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
//     mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
//     ADD phase_add = mgr->constant(phase_complex);
//     mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);


//     ADD CPGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
//                  + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
//                  + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * x_vars[controlled]
//                  + y_vars[controller] * y_vars[controlled] * x_vars[controller] * ~x_vars[controlled] * phase_add;
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
//     tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
//     stateVector = CPGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     CUDD_VALUE_TYPE phase_complex;
//     mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
//     mpfr_const_pi(phase_complex.real, RND_TYPE);
//     mpfr_const_pi(phase_complex.imag, RND_TYPE);
//     mpfr_mul_d(phase_complex.real, phase_complex.real, theta, RND_TYPE);
//     mpfr_mul_d(phase_complex.imag, phase_complex.imag, theta, RND_TYPE);
//     mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
//     mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
//     ADD phase_add = mgr->constant(phase_complex);
//     mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);

//     ADD PhaseShiftGate =  ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index] * phase_add;
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]);
//     stateVector = PhaseShiftGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyTGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     CUDD_VALUE_TYPE phase_complex;
//     mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
//     mpfr_const_pi(phase_complex.real, RND_TYPE);
//     mpfr_const_pi(phase_complex.imag, RND_TYPE);
//     mpfr_mul_d(phase_complex.real, phase_complex.real, 0.25, RND_TYPE);
//     mpfr_mul_d(phase_complex.imag, phase_complex.imag, 0.25, RND_TYPE);
//     mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
//     mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
//     ADD phase_add = mgr->constant(phase_complex);
//     mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);

//     ADD TGate =  ~y_vars[index] * ~x_vars[index] + y_vars[index] * x_vars[index] * phase_add;
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[index]); tmp_y.push_back(y_vars[index]); 
//     stateVector = TGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyCSGate(long int controller, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }

//     CUDD_VALUE_TYPE phase_complex;
//     mpfr_init(phase_complex.real); mpfr_init(phase_complex.imag);
//     mpfr_const_pi(phase_complex.real, RND_TYPE);
//     mpfr_const_pi(phase_complex.imag, RND_TYPE);
//     mpfr_mul_d(phase_complex.real, phase_complex.real, 0.5, RND_TYPE);
//     mpfr_mul_d(phase_complex.imag, phase_complex.imag, 0.5, RND_TYPE);
//     mpfr_cos(phase_complex.real, phase_complex.real, RND_TYPE);
//     mpfr_sin(phase_complex.imag, phase_complex.imag, RND_TYPE);
//     ADD phase_add = mgr->constant(phase_complex);
//     mpfr_clear(phase_complex.real); mpfr_clear(phase_complex.imag);

//     ADD CSGate = ~y_vars[controller] * ~y_vars[controlled] * ~x_vars[controller] * ~x_vars[controlled]
//                  + ~y_vars[controller] * y_vars[controlled] * ~x_vars[controller] * x_vars[controlled]
//                  + y_vars[controller] * ~y_vars[controlled] * x_vars[controller] * ~x_vars[controlled]
//                  + y_vars[controller] * y_vars[controlled] * x_vars[controller] * x_vars[controlled] * phase_add;
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[controlled]);
//     tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[controlled]);
//     stateVector = CSGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyCCNOTGate(long int controller, long int index1, long int index2)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
    
//     ADD CCNOTGate = ~y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
//                   + ~y_vars[controller] * ~y_vars[index1] * y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * x_vars[index2]
//                   + ~y_vars[controller] * y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * x_vars[index1] * ~x_vars[index2]
//                   + ~y_vars[controller] * y_vars[index1] * y_vars[index2] * ~x_vars[controller] * x_vars[index1] * x_vars[index2]
//                   + y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
//                   + y_vars[controller] * ~y_vars[index1] * y_vars[index2] * x_vars[controller] * ~x_vars[index1] * x_vars[index2]
//                   + y_vars[controller] * y_vars[index1] * ~y_vars[index2] * x_vars[controller] * x_vars[index1] * x_vars[index2]
//                   + y_vars[controller] * y_vars[index1] * y_vars[index2] * x_vars[controller] * x_vars[index1] * ~x_vars[index2];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
//     tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
//     stateVector = CCNOTGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// void BDDQuantumCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ADD CSwapGate = ~y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
//                   + ~y_vars[controller] * ~y_vars[index1] * y_vars[index2] * ~x_vars[controller] * ~x_vars[index1] * x_vars[index2]
//                   + ~y_vars[controller] * y_vars[index1] * ~y_vars[index2] * ~x_vars[controller] * x_vars[index1] * ~x_vars[index2]
//                   + ~y_vars[controller] * y_vars[index1] * y_vars[index2] * ~x_vars[controller] * x_vars[index1] * x_vars[index2]
//                   + y_vars[controller] * ~y_vars[index1] * ~y_vars[index2] * x_vars[controller] * ~x_vars[index1] * ~x_vars[index2]
//                   + y_vars[controller] * ~y_vars[index1] * y_vars[index2] * x_vars[controller] * x_vars[index1] * ~x_vars[index2]
//                   + y_vars[controller] * y_vars[index1] * ~y_vars[index2] * x_vars[controller] * ~x_vars[index1] * x_vars[index2]
//                   + y_vars[controller] * y_vars[index1] * y_vars[index2] * x_vars[controller] * x_vars[index1] * x_vars[index2];
//     std::vector<ADD> tmp_x, tmp_y; 
//     tmp_x.push_back(x_vars[controller]); tmp_x.push_back(x_vars[index1]); tmp_x.push_back(x_vars[index2]);
//     tmp_y.push_back(y_vars[controller]); tmp_y.push_back(y_vars[index1]); tmp_y.push_back(y_vars[index2]);
//     stateVector = CSwapGate.MatrixMultiply(stateVector, tmp_x);
//     stateVector = stateVector.SwapVariables(tmp_y, tmp_x);
// }

// long double BDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
// {
//     // stateVector.print(2*numQubits, 2);
//     ADD tmp = stateVector.SquareTerminalValues();
//     ADD s_add = mgr->addOne();

//     for (auto it = qubit_vals.begin(); it != qubit_vals.end(); it++)
//     {
//         if (it->second == 0)
//             s_add *= ~x_vars[it->first];
//         else
//             s_add *= x_vars[it->first];
//     }
//     tmp = tmp * s_add;
//     tmp.UpdatePathInfo(2, numQubits);
//     // tmp.PrintPathInfo();
//     return tmp.GetProbability(2, numQubits);
// }

// std::string BDDQuantumCircuit::Measure() 
// {
//     ADD tmp = stateVector.SquareTerminalValues();
//     tmp.UpdatePathInfo(2, numQubits);
//     return tmp.SamplePath(numQubits, 2, "").substr(0, numQubits); 
// }

// unsigned long long int BDDQuantumCircuit::GetPathCount(long double prob)
// {
//     ADD tmp = stateVector.SquareTerminalValues();
//     tmp.UpdatePathInfo(2, numQubits);
//     return tmp.GetPathCount(numQubits, 2, prob); 
// }

// // *******************
// // Weighted BDD
// // *******************

// #include "cflobdd/CFLOBDD/wvector_complex_fb_mul.h"
// #include "cflobdd/CFLOBDD/weighted_cross_product.h"
// #include "cflobdd/CFLOBDD/weighted_cross_product_bdd.h"

// WeightedBDDQuantumCircuit::WeightedBDDQuantumCircuit(unsigned int numQubits, int seed) : QuantumCircuit(numQubits, seed)
// {
//     // Initialize
//     WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable();
// 	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable_Ann();
// 	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitIdentityNodeTable();	
// 	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitReduceCache();
// 	WeightedMatrix1234ComplexFloatBoostMul::Matrix1234Initializer();
// 	WeightedVectorComplexFloatBoostMul::VectorInitializer();
// 	InitWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();

//     WeightedBDDNodeHandle<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitLeafNodes();
// 	InitWeightedBDDPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
//     //
//     std::string s(2 * numQubits, '0');
//     stateVector = WeightedVectorComplexFloatBoostMul::MkBasisVector(2 * numQubits, s, 0);
// }

// WeightedBDDQuantumCircuit::WeightedBDDQuantumCircuit()
// {
//     // Initialize
//     WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable();
// 	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitNoDistinctionTable_Ann();
// 	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitIdentityNodeTable();	
// 	WeightedCFLOBDDNodeHandleT<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitReduceCache();
// 	WeightedMatrix1234ComplexFloatBoostMul::Matrix1234Initializer();
// 	WeightedVectorComplexFloatBoostMul::VectorInitializer();
// 	InitWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();

//     WeightedBDDNodeHandle<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>::InitLeafNodes();
// 	InitWeightedBDDPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
//     numQubits = 0;
//     //
// }

// WeightedBDDQuantumCircuit::~WeightedBDDQuantumCircuit()
// {
//     DisposeOfWeightedPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
//     DisposeOfWeightedBDDPairProductCache<BIG_COMPLEX_FLOAT, std::multiplies<BIG_COMPLEX_FLOAT>>();
// }

// void WeightedBDDQuantumCircuit::setNumQubits(unsigned int num)
// {
//     numQubits = num;
//     stateVector = WeightedVectorComplexFloatBoostMul::MkBasisVector(numQubits, 0, 0);
// }

// WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateF(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, int))
// {
//     WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = f(2, 0);
//     if (i == 0)
//     {
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0);
//         return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I); 
//     }
//     else if (i == n - 1)
//     {
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0);
//         return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I, H);
//     }
//     else {
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I1 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * i, 0);
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I2 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n - i - 1), 0);
//         auto T = WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I2);
//         return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I1, T);
//     }
// }

// WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateFWithParam(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, double, int), double theta)
// {
//     WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = f(2, theta, 0);
//     if (i == 0)
//     {
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0);
//         return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I); 
//     }
//     else if (i == n - 1)
//     {
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n-1), 0);
//         return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I, H);
//     }
//     else {
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I1 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * i, 0);
//         WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL I2 = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(2 * (n - i - 1), 0);
//         auto T = WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, I2);
//         return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(I1, T);
//     }
// }


// void WeightedBDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto H = ApplyGateF(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(H, stateVector);
// }

// void WeightedBDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto H = ApplyGateF(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkWalshInterleaved);
//     // H.print(std::cout);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(H, stateVector);
//     // stateVector.print(std::cout);
//     // std::cout << std::endl;
//     hadamard_count++;
// }

// void WeightedBDDQuantumCircuit::ApplyNOTGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     // stateVector.print(std::cout);
//     auto X = ApplyGateF(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkNegationMatrixInterleaved);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(X, stateVector);
//     // stateVector.print(std::cout);
// }

// void WeightedBDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto Y = ApplyGateF(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliYGate);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Y, stateVector);
// }

// void WeightedBDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto Z = ApplyGateF(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliZGate);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Z, stateVector);
// }

// void WeightedBDDQuantumCircuit::ApplySGate(unsigned int index)
// {
//     auto S = ApplyGateF(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkSGate);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector);
// }

// void WeightedBDDQuantumCircuit::ApplyCNOTGate(long int controller, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(controller != controlled);
    
//     if (controller < controlled)
//     {
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(2 * numQubits, numQubits, controller, controlled, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//         // stateVector.print(std::cout);
//     }
//     else
//     {
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller, 0);
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCNOT(2 * numQubits, numQubits, controlled, controller, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
// }

// void WeightedBDDQuantumCircuit::ApplySwapGate(long int index1, long int index2)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(index1 != index2);
    
//     if (index1 < index2)
//     {
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index1, index2, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else
//     {
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index2, index1, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
// }

// void WeightedBDDQuantumCircuit::ApplyiSwapGate(long int index1, long int index2)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(index1 != index2);
    
//     if (index1 < index2)
//     {
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkiSwapGate(2 * numQubits, index1, index2, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else
//     {
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkiSwapGate(2 * numQubits, index2, index1, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
// }

// void WeightedBDDQuantumCircuit::ApplyCZGate(long int controller, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(controller != controlled);
    
//     if (controller < controlled)
//     {
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controller, controlled, 1.0, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else
//     {
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller, 0);
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controlled, controller, 1.0, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
// }

// void WeightedBDDQuantumCircuit::ApplyCPGate(long int controller, long int controlled, double theta)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(controller != controlled);
    
//     if (controller < controlled)
//     {
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controller, controlled, theta, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else
//     {
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller, 0);
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCPGate(2 * numQubits, controlled, controller, theta, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     } 
// }

// void WeightedBDDQuantumCircuit::ApplyCSGate(long int controller, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     ApplyCPGate(controller, controlled, 0.5);
// }

// void WeightedBDDQuantumCircuit::ApplyPhaseShiftGate(unsigned int index, double theta)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto S = ApplyGateFWithParam(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, theta);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector); 
// }

// void WeightedBDDQuantumCircuit::ApplyTGate(unsigned int index)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto S = ApplyGateFWithParam(numQubits, index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, 0.25);
//     stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector); 
// }

// void WeightedBDDQuantumCircuit::ApplyCCNOTGate(long int controller1, long int controller2, long int controlled)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(controller1 != controlled);
//     assert(controller2 != controlled);
//     assert(controller1 != controller2);
//     if (controller1 < controller2 && controller2 < controlled)
//     {
//         // a b c
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controller1, controller2, controlled, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (controller1 < controlled && controlled < controller2)
//     {
//         // a c b   
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller2, 0);
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controller1, controlled, controller2, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (controller2 < controller1 && controller1 < controlled)
//     {
//         // b a c
//         ApplyCCNOTGate(controller2, controller1, controlled);
//     }
//     else if (controller2 < controlled && controlled < controller1)
//     {
//         // b c a
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller1, 0);
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controller2, controlled, controller1, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector); 
//     }
//     else if (controlled < controller1 && controller1 < controller2)
//     {
//         // c a b
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller2, 0);
//         // b a c
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controlled, controller1, controller2, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (controlled < controller2 && controller2 < controller1)
//     {
//         // c b a
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, controlled, controller1, 0);
//         // a b c
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCCNOT(2 * numQubits, controlled, controller2, controller1, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
// }

// void WeightedBDDQuantumCircuit::ApplyCSwapGate(long int controller, long int index1, long int index2)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     assert(controller != index1);
//     assert(controller != index2);
//     assert(index1 != index2);
    
//     if (controller < index1 && index1 < index2)
//     {
//         // a b c
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, controller, index1, index2, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (controller < index2 && index2 < index1)
//     {
//         // a c b   
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, controller, index2, index1, 0);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (index1 < controller && controller < index2)
//     {
//         // b a c
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index1, controller, 0);
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index1, controller, index2, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (index1 < index2 && index2 < controller)
//     {
//         // b c a
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index1, controller, 0);
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index1, index2, controller, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (index2 < controller && controller < index1)
//     {
//         // c a b
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index2, controller, 0);
//         // b a c
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index2, controller, index1, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
//     else if (index2 < index1 && index1 < controller)
//     {
//         // c b a
//         auto S = WeightedMatrix1234ComplexFloatBoostMul::MkSwapGate(2 * numQubits, index2, controller, 0);
//         // a b c
//         auto C = WeightedMatrix1234ComplexFloatBoostMul::MkCSwapGate(2 * numQubits, index2, index1, controller, 0);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, S);
//         C = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, C);
//         stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(C, stateVector);
//     }
// }

// void WeightedBDDQuantumCircuit::ApplyGlobalPhase(double phase)
// {
//     if (checkForInit(numQubits) == false)
//     {
//         std::cout << "Number of Qubits is unset" << std::endl;
//         abort();   
//     }
//     auto cos_v = boost::math::cos_pi(phase);
//     auto sin_v = boost::math::sin_pi(phase);
//     BIG_COMPLEX_FLOAT phase_complex(cos_v, sin_v);
//     stateVector = phase_complex * stateVector;
// }

// long double WeightedBDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
// {
//     auto tmp = stateVector;
//     std::string s(numQubits, 'X');
//     for (unsigned int i = 0; i < numQubits; i++)
//     {
//         if (qubit_vals.find(i) != qubit_vals.end())
//         {
//             if (qubit_vals[i] == 0)
//                 s[i] = '0';
//             else if (qubit_vals[i] == 1)
//                 s[i] = '1';   
//         }
//     }
//     auto restricted = WeightedMatrix1234ComplexFloatBoostMul::MkRestrictMatrix(2 * numQubits, s, 0);
//     tmp = tmp * restricted;
//     tmp.ComputeWeightOfPathsAsAmpsToExits();
//     return WeightedVectorComplexFloatBoostMul::getNonZeroProbability(tmp);
// }

// std::string WeightedBDDQuantumCircuit::Measure() 
// {
//     auto tmp = stateVector;
//     // tmp.print(std::cout);
//     tmp.ComputeWeightOfPathsAsAmpsToExits();
//     std::uniform_real_distribution<double> dis(0.0, 1.0);
//     return WeightedVectorComplexFloatBoostMul::Sampling(tmp, true, mt, dis).substr(0, numQubits); 
// }

// unsigned long long int WeightedBDDQuantumCircuit::GetPathCount(long double prob)
// {
//     std::cout << "Error! Operation not supported in WBDDs" << std::endl;
//     abort();
// } 

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

WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateF(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int))
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
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = ApplyGateF(n/2, i, f);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, T);
        }
        else
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(T, ApplyGateF(n/2, i - n/2, f)); 
        }
    }
}

WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL ApplyGateFWithParam(unsigned int n, unsigned int i, WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL(*f)(unsigned int, double), double theta)
{
    if (n == 1)
    {
        return f(1, theta);
    }
    else {
        int level = ceil(log2(n/2));
        if (i < n/2)
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL H = ApplyGateFWithParam(n/2, i, f, theta);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(H, T);
        }
        else
        {
            WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL T = WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved(level + 1);
            return WeightedMatrix1234ComplexFloatBoostMul::KroneckerProduct2Vocs(T, ApplyGateFWithParam(n/2, i - n/2, f, theta)); 
        }
    }
}


void WeightedCFLOBDDQuantumCircuit::ApplyIdentityGate(unsigned int index)
{
    auto H = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkIdRelationInterleaved);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(H, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyHadamardGate(unsigned int index)
{
    auto H = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkWalshInterleaved);
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
    tmp.ComputeWeightOfPathsAsAmpsToExits();
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return WeightedVectorComplexFloatBoostMul::Sampling(tmp, true).substr(0, numQubits); 
}

void WeightedCFLOBDDQuantumCircuit::ApplyNOTGate(unsigned int index)
{
    auto X = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkNegationMatrixInterleaved);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(X, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyPauliYGate(unsigned int index)
{
    auto Y = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliYGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Y, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyPauliZGate(unsigned int index)
{
    auto Z = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPauliZGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(Z, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplySXGate(unsigned int index)
{
    auto SX = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkSXGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(SX, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplySYGate(unsigned int index)
{
    auto SY = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkSYGate);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(SY, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplySGate(unsigned int index)
{
    auto S = ApplyGateF(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkSGate);
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
    auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, theta);
    stateVector = WeightedMatrix1234ComplexFloatBoostMul::MatrixMultiplyV4(S, stateVector);
}

void WeightedCFLOBDDQuantumCircuit::ApplyTGate(unsigned int index)
{
    auto S = ApplyGateFWithParam(std::pow(2, stateVector.root->level-1), index, WeightedMatrix1234ComplexFloatBoostMul::MkPhaseShiftGate, 0.25);
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

long double WeightedCFLOBDDQuantumCircuit::GetProbability(std::map<unsigned int, int>& qubit_vals)
{
}

unsigned long long int WeightedCFLOBDDQuantumCircuit::GetPathCount(long double prob)
{
    std::cout << "Error! Operation not supported in WBDDs" << std::endl;
    abort();
} 

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




