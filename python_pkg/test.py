import pyqcmc

numQubits = 20
qc = pyqcmc.QuantumVerifier(numQubits)

qc.h(0)


for i in range(1, numQubits):
    qc.cx(0, i)

qubit_vals = {}
for i in range(0, numQubits):
    qubit_vals[i] = 0

print(qc.prob(qubit_vals))