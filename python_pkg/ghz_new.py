import sys
import quasimodo
import time
import random

numQubits = int(sys.argv[1])

random.seed(int(sys.argv[3]))

allZeros = ""
allOnes = ""
for i in range(0, numQubits+1):
    allZeros += "0"
    allOnes += "1"

start = time.time()
qc = quasimodo.QuantumCircuit(sys.argv[2], numQubits+1, int(sys.argv[3]))

indices = ["0" for i in range(0, numQubits+1)]
for i in range(numQubits):
    indices[i] = "1"

h_gate = qc.create_h(''.join(indices))


qc.x(numQubits)

qc.apply_gate(h_gate)

cx_gate = qc.create_cx(0, numQubits)
for i in range(1, numQubits):
    tmp = qc.create_cx(i, numQubits)
    cx_gate = qc.gate_gate_apply(cx_gate, tmp)

qc.apply_gate(cx_gate)

indices = ["1" for i in range(0, numQubits+1)]
h_gate_2 = qc.create_h(''.join(indices))

qc.apply_gate(h_gate_2)

sampled_string = qc.measure()
sample_count = 1

iter_count = 0


while (not (sampled_string == allZeros or sampled_string == allOnes)) and iter_count < numQubits:
    sample_count += 1
    sampled_string = qc.measure()
    iter_count += 1

end = time.time()

memory = qc.size()

if iter_count >= numQubits:
    print ('Incorrect , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)
else :
    print ('Correct , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)

