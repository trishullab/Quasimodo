#ifndef _QUANTUM_CIRCUIT
#define _QUANTUM_CIRCUIT

#include "cflobdd/CFLOBDD/matrix1234_complex_float_boost.h"
#include "quantum_gate.h"
#include "quantum_state.h"
#include <random>


class QuantumCircuit {
    public:
        // Constructor
        QuantumCircuit(unsigned int numQubits, int seed);
        // Constructor
        QuantumCircuit();
        // Destructor
        virtual ~QuantumCircuit();
        // set qubit count;
        virtual void setNumQubits(unsigned int numQubits) = 0;
        // I = [[1 0] [0 1]]
        // For no-op
        virtual void ApplyIdentityGate(unsigned int index) = 0;
        // H = [[1 1] [1 -1]]
        // Also called Walsh Gate
        virtual void ApplyHadamardGate(unsigned int index) = 0;
        // X = [[0 1] [1 0]]
        // Also called Pauli-X or bit-flip
        virtual void ApplyNOTGate(unsigned int index) = 0;
        // Y = [[0 -i] [i 0]]
        virtual void ApplyPauliYGate(unsigned int index) = 0;
        // Z = [[1 0] [0 -1]]
        // Also called phase-flip
        virtual void ApplyPauliZGate(unsigned int index) = 0;
        // S = [[1 0] [0 i]]
        // Also called √(Z) Gate
        virtual void ApplySGate(unsigned int index) = 0;
        // CNOT = [[1 0 0 0] [0 1 0 0] [0 0 0 1] [0 0 1 0]]
        virtual void ApplyCNOTGate(long int controller, long int controlled) = 0;
        // Ph = e^{i * π * phase} I
        virtual void ApplyGlobalPhase(double phase) = 0;
        // SWAP = [[1 0 0 0] [0 0 1 0] [0 1 0 0] [0 0 0 1]]
        virtual void ApplySwapGate(long int index1, long int index2) = 0;
        // iSWAP = [[1 0 0 0] [0 0 i 0] [0 i 0 0] [0 0 0 1]]
        virtual void ApplyiSwapGate(long int index1, long int index2) = 0;
        // CZ = [[1 0 0 0] [0 1 0 0] [0 0 1 0] [0 0 0 -1]]
        // Also called Controlled Phase Flip Gate
        virtual void ApplyCZGate(long int controller, long int controlled) = 0;
        // CPhase = [[1 0 0 0] [0 1 0 0] [0 0 1 0] [0 0 0 e^(i * π * θ)]]
        virtual void ApplyCPGate(long int controller, long int controlled, double theta) = 0;
        // P = [[1 0] [0 e^{i * π * θ}]]
        virtual void ApplyPhaseShiftGate(unsigned int index, double theta) = 0;
        // T = P(1/4) = [[1 0] [0 e^{i * π * 1/4}]]
        virtual void ApplyTGate(unsigned int index) = 0;
        // CS = [[1 0 0 0] [0 1 0 0] [0 0 1 0] [0 0 0 e^(i * π * 1/2)]]
        virtual void ApplyCSGate(long int controller, long int controlled) = 0;
        // CCNOT or Toffoli gate
        virtual void ApplyCCNOTGate(long int controller1, long int controller2, long int controlled) = 0;
        // CSWAP or Fredkin gate
        virtual void ApplyCSwapGate(long int controller, long int index1, long int index2) = 0; 
        // SX = √X = [[1 + i 1 - i] [1 - i 1 + i]]
        virtual void ApplySXGate(unsigned int index) = 0; 
        // SY = √Y = [[1 + i -1 - i] [1 + i 1 + i]]
        virtual void ApplySYGate(unsigned int index) = 0; 
        // CCP = controlled-controlled-phase
        virtual void ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta) = 0; 
        virtual void ApplyMCXGate(std::vector<long int> controllers, long int controllerd) = 0;
        // Obtain Probability
        virtual long double GetProbability(std::map<unsigned int, int>& qubit_vals) = 0;
        // Measure
        virtual std::string Measure() = 0;
        // Measure and Collapse
        virtual std::string MeasureAndCollapse(std::vector<long int>& indices) = 0;
        // Get Path Counts
        virtual unsigned long long int GetPathCount(long double prob) = 0;
        virtual unsigned int Size() = 0;

        virtual QuantumGate* CreateHadamardGate(std::string indices) = 0;
        virtual QuantumGate* CreateNOTGate(std::string indices) = 0;
        virtual QuantumGate* CreateCNOTGate(long int controller, long int controlled) = 0;
        virtual QuantumState* GetState() = 0;
    
    protected:
        unsigned int numQubits;
        unsigned int hadamard_count;
        std::mt19937_64 mt;
};

using namespace CFL_OBDD;

class CFLOBDDQuantumCircuit : public QuantumCircuit {
    public:
        CFLOBDDQuantumCircuit(unsigned int numQubits,  int seed);
        CFLOBDDQuantumCircuit();
        ~CFLOBDDQuantumCircuit();
        void setNumQubits(unsigned int numQubits);
        void ApplyIdentityGate(unsigned int index);
        void ApplyHadamardGate(unsigned int index);
        void ApplyNOTGate(unsigned int index);
        void ApplyPauliYGate(unsigned int index);
        void ApplyPauliZGate(unsigned int index);
        void ApplySGate(unsigned int index);
        void ApplyCNOTGate(long int controller, long int controlled);
        void ApplyGlobalPhase(double phase);
        void ApplySwapGate(long int index1, long int index2);
        void ApplyiSwapGate(long int index1, long int index2);
        void ApplyCZGate(long int controller, long int controlled);
        void ApplyCPGate(long int controller, long int controlled, double theta);
        void ApplyPhaseShiftGate(unsigned int index, double theta);
        void ApplyTGate(unsigned int index);
        void ApplyCSGate(long int controller, long int controlled);
        void ApplyCCNOTGate(long int controller1, long int controller2, long int controlled);
        void ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta);
        void ApplyMCXGate(std::vector<long int> controllers, long int controlled);
        void ApplyCSwapGate(long int controller, long int index1, long int index2); 
        void ApplySXGate(unsigned int index);
        void ApplySYGate(unsigned int index);
        void ApplyCUGate(long int a, long int N);
        long double GetProbability(std::map<unsigned int, int>& qubit_vals);
        std::string Measure();
        std::string MeasureAndCollapse(std::vector<long int>& indices);
        unsigned long long int GetPathCount(long double prob);
        unsigned int Size();


        CFLOBDDQuantumGate* CreateHadamardGate(std::string indices);
        CFLOBDDQuantumGate* CreateIdentityGate(std::string indices);
        CFLOBDDQuantumGate* CreateNOTGate(std::string indices);
        CFLOBDDQuantumGate* CreateCNOTGate(long int controller, long int controlled);
        CFLOBDDQuantumGate* KroneckerProduct(CFLOBDDQuantumGate* m1, CFLOBDDQuantumGate* m2);
        CFLOBDDQuantumGate* GateGateApply(CFLOBDDQuantumGate* m1, CFLOBDDQuantumGate* m2);
        void ApplyGate(CFLOBDDQuantumGate* m);
        CFLOBDDQuantumState* GetState();
    private:
        CFLOBDD_COMPLEX_BIG stateVector;
        //unsigned int numQubits;
};

#include "cflobdd/cudd-complex-big/cplusplus/cuddObj.hh"

class BDDQuantumCircuit : public QuantumCircuit {
    public:
        BDDQuantumCircuit(unsigned int numQubits, int seed);
        BDDQuantumCircuit();
        ~BDDQuantumCircuit();
        void setNumQubits(unsigned int numQubits);
        void ApplyIdentityGate(unsigned int index);
        void ApplyHadamardGate(unsigned int index);
        void ApplyNOTGate(unsigned int index);
        void ApplyPauliYGate(unsigned int index);
        void ApplyPauliZGate(unsigned int index);
        void ApplySGate(unsigned int index);
        void ApplyCNOTGate(long int controller, long int controlled);
        void ApplyGlobalPhase(double phase);
        void ApplySwapGate(long int index1, long int index2);
        void ApplyiSwapGate(long int index1, long int index2);
        void ApplyCZGate(long int controller, long int controlled);
        void ApplyCPGate(long int controller, long int controlled, double theta);
        void ApplyPhaseShiftGate(unsigned int index, double theta);
        void ApplyTGate(unsigned int index);
        void ApplyCSGate(long int controller, long int controlled);
        void ApplyCCNOTGate(long int controller1, long int controller2, long int controlled);
        void ApplyMCXGate(std::vector<long int> controllers, long int controlled);
        void ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta);
        void ApplyCSwapGate(long int controller, long int index1, long int index2); 
        void ApplySXGate(unsigned int index);
        void ApplySYGate(unsigned int index);
        void ApplyCUGate(long int a, long int N);
        long double GetProbability(std::map<unsigned int, int>& qubit_vals);
        std::string Measure();
        std::string MeasureAndCollapse(std::vector<long int>& indices);
        unsigned long long int GetPathCount(long double prob);
        unsigned int Size();

        BDDQuantumGate* CreateHadamardGate(std::string indices);
        BDDQuantumGate* CreateIdentityGate(std::string indices);
        BDDQuantumGate* CreateNOTGate(std::string indices);
        BDDQuantumGate* CreateCNOTGate(long int controller, long int controlled);
        BDDQuantumGate* KroneckerProduct(BDDQuantumGate* m1, BDDQuantumGate* m2);
        BDDQuantumGate* GateGateApply(BDDQuantumGate* m1, BDDQuantumGate* m2);
        void ApplyGate(BDDQuantumGate* m);
        BDDQuantumState* GetState();
    private:
        ADD stateVector;
        Cudd* mgr;
        std::vector<ADD> x_vars;
        std::vector<ADD> y_vars;
        std::vector<ADD> z_vars;
        //unsigned int numQubits;
};

#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"

class WeightedBDDQuantumCircuit : public QuantumCircuit
{
    public:
        // Constructor
        WeightedBDDQuantumCircuit(unsigned int numQubits, int seed);
        // Constructor
        WeightedBDDQuantumCircuit();
        // Destructor
        ~WeightedBDDQuantumCircuit();
        // set qubit count;
        void setNumQubits(unsigned int numQubits);
        void ApplyIdentityGate(unsigned int index);
        void ApplyHadamardGate(unsigned int index);
        void ApplyNOTGate(unsigned int index);
        void ApplyPauliYGate(unsigned int index);
        void ApplyPauliZGate(unsigned int index);
        void ApplySGate(unsigned int index);
        void ApplyCNOTGate(long int controller, long int controlled);
        void ApplyGlobalPhase(double phase);
        void ApplySwapGate(long int index1, long int index2);
        void ApplyiSwapGate(long int index1, long int index2);
        void ApplyCZGate(long int controller, long int controlled);
        void ApplyCPGate(long int controller, long int controlled, double theta);
        void ApplyPhaseShiftGate(unsigned int index, double theta);
        void ApplyTGate(unsigned int index);
        void ApplyCSGate(long int controller, long int controlled);
        void ApplyCCNOTGate(long int controller1, long int controller2, long int controlled);
        void ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta);
        void ApplyMCXGate(std::vector<long int> controllers, long int controllerd);
        void ApplyCSwapGate(long int controller, long int index1, long int index2); 
        void ApplySXGate(unsigned int index);
        void ApplySYGate(unsigned int index);
        void ApplyCUGate(long int a, long int N);
        long double GetProbability(std::map<unsigned int, int>& qubit_vals);
        std::string Measure();
        std::string MeasureAndCollapse(std::vector<long int>& indices);
        unsigned long long int GetPathCount(long double prob);
        unsigned int Size();

        WeightedBDDQuantumGate* CreateHadamardGate(std::string indices);
        WeightedBDDQuantumGate* CreateIdentityGate(std::string indices);
        WeightedBDDQuantumGate* CreateNOTGate(std::string indices);
        WeightedBDDQuantumGate* CreateCNOTGate(long int controller, long int controlled);
        
        WeightedBDDQuantumGate* KroneckerProduct(WeightedBDDQuantumGate* m1, WeightedBDDQuantumGate* m2);
        WeightedBDDQuantumGate* GateGateApply(WeightedBDDQuantumGate* m1, WeightedBDDQuantumGate* m2);
        void ApplyGate(WeightedBDDQuantumGate* m);
        WeightedBDDQuantumState* GetState();
    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL stateVector;
};

#include "cflobdd/CFLOBDD/wmatrix1234_complex_fb_mul.h"

class WeightedCFLOBDDQuantumCircuit : public QuantumCircuit {
    public:
        WeightedCFLOBDDQuantumCircuit(unsigned int numQubits,  int seed);
        WeightedCFLOBDDQuantumCircuit();
        ~WeightedCFLOBDDQuantumCircuit();
        void setNumQubits(unsigned int numQubits);
        void ApplyIdentityGate(unsigned int index);
        void ApplyHadamardGate(unsigned int index);
        void ApplyNOTGate(unsigned int index);
        void ApplyPauliYGate(unsigned int index);
        void ApplyPauliZGate(unsigned int index);
        void ApplySGate(unsigned int index);
        void ApplyCNOTGate(long int controller, long int controlled);
        void ApplyGlobalPhase(double phase);
        void ApplySwapGate(long int index1, long int index2);
        void ApplyiSwapGate(long int index1, long int index2);
        void ApplyCZGate(long int controller, long int controlled);
        void ApplyCPGate(long int controller, long int controlled, double theta);
        void ApplyPhaseShiftGate(unsigned int index, double theta);
        void ApplyTGate(unsigned int index);
        void ApplyCSGate(long int controller, long int controlled);
        void ApplyCCNOTGate(long int controller1, long int controller2, long int controlled);
        void ApplyCSwapGate(long int controller, long int index1, long int index2); 
        void ApplySXGate(unsigned int index);
        void ApplySYGate(unsigned int index);
        void ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta);
        void ApplyMCXGate(std::vector<long int> controllers, long int controlled);
        long double GetProbability(std::map<unsigned int, int>& qubit_vals);
        std::string Measure();
        std::string MeasureAndCollapse(std::vector<long int>& indices);
        unsigned long long int GetPathCount(long double prob);
        unsigned int Size();

        WeightedCFLOBDDQuantumGate* CreateHadamardGate(std::string indices);
        WeightedCFLOBDDQuantumGate* CreateIdentityGate(std::string indices);
        WeightedCFLOBDDQuantumGate* CreateNOTGate(std::string indices);
        WeightedCFLOBDDQuantumGate* CreateCNOTGate(long int controller, long int controlled);
        
        WeightedCFLOBDDQuantumGate* KroneckerProduct(WeightedCFLOBDDQuantumGate* m1, WeightedCFLOBDDQuantumGate* m2);
        WeightedCFLOBDDQuantumGate* GateGateApply(WeightedCFLOBDDQuantumGate* m1, WeightedCFLOBDDQuantumGate* m2);
        void ApplyGate(WeightedCFLOBDDQuantumGate* m);
        WeightedCFLOBDDQuantumState* GetState();
    private:
        WEIGHTED_CFLOBDD_COMPLEX_FLOAT_BOOST_MUL stateVector;
        //unsigned int numQubits;
};

#include <memory>
#include "dd_package/include/dd/Package.hpp"
using namespace dd;

class MQTDDCircuit : public QuantumCircuit {
    public:
        MQTDDCircuit(unsigned int numQubits,  int seed);
        MQTDDCircuit();
        ~MQTDDCircuit();
        void setNumQubits(unsigned int numQubits);
        void ApplyIdentityGate(unsigned int index);
        void ApplyHadamardGate(unsigned int index);
        void ApplyNOTGate(unsigned int index);
        void ApplyPauliYGate(unsigned int index);
        void ApplyPauliZGate(unsigned int index);
        void ApplySGate(unsigned int index);
        void ApplyCNOTGate(long int controller, long int controlled);
        void ApplyGlobalPhase(double phase);
        void ApplySwapGate(long int index1, long int index2);
        void ApplyiSwapGate(long int index1, long int index2);
        void ApplyCZGate(long int controller, long int controlled);
        void ApplyCPGate(long int controller, long int controlled, double theta);
        void ApplyPhaseShiftGate(unsigned int index, double theta);
        void ApplyTGate(unsigned int index);
        void ApplyCSGate(long int controller, long int controlled);
        void ApplyCCNOTGate(long int controller1, long int controller2, long int controlled);
        void ApplyCSwapGate(long int controller, long int index1, long int index2);
        void ApplySXGate(unsigned int index);
        void ApplySYGate(unsigned int index);
        void ApplyCCPGate(long int controller1, long int controller2, long int controlled, double theta);
        void ApplyMCXGate(std::vector<long int> controllers, long int controlled);
        long double GetProbability(std::map<unsigned int, int>& qubit_vals);
        std::string Measure();
        std::string MeasureAndCollapse(std::vector<long int>& indices);
        unsigned long long int GetPathCount(long double prob);
        unsigned int Size();

        MQTDDQuantumGate* CreateHadamardGate(std::string indices);
        MQTDDQuantumGate* CreateIdentityGate(std::string indices);
        MQTDDQuantumGate* CreateNOTGate(std::string indices);
        MQTDDQuantumGate* CreateCNOTGate(long int controller, long int controlled);
        
        MQTDDQuantumGate* KroneckerProduct(MQTDDQuantumGate* m1, MQTDDQuantumGate* m2);
        MQTDDQuantumGate* GateGateApply(MQTDDQuantumGate* m1, MQTDDQuantumGate* m2);
        void ApplyGate(MQTDDQuantumGate* m);
        MQTDDQuantumState* GetState();
    private:
        std::unique_ptr<Package<DDPackageConfig>> ddp;
        vEdge stateVector;
        //unsigned int numQubits;
};

#endif
