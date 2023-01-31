import pyqcmc

"""
    model_str : str -> {"CFLOBDD", "BDD"}
    numQubits: unsigned int
"""
def QuantumCircuitModelChecker(model_str = "CFLOBDD", numQubits = 0, seed = 0):
    if model_str == "CFLOBDD":
        return pyqcmc.CFLOBDDQuantumVerifier(numQubits, seed)
    elif model_str == "BDD":
        return pyqcmc.BDDQuantumVerifier(numQubits, seed)
    elif model_str == "WBDD":
        return pyqcmc.WeightedBDDQuantumVerifier(numQubits, seed)