import sys
import quasimodo
import random
import time

# argv[1] -> numQubits
# argv[2] -> CFLOBDD/BDD
numQubits = int(sys.argv[1])

random.seed(int(sys.argv[3]))

s = ""
for i in range(0, numQubits):
    r = random.randint(0, 1)
    if r == 0:
        s = s + '0'
    else:
        s = s + '1'


allZeros = ""
allOnes = ""
for i in range(0, numQubits):
    allZeros += "0"
    allOnes += "1"


qc = quasimodo.QuantumCircuit(sys.argv[2], numQubits + 1, int(sys.argv[3]))

indices = ["0" for i in range(0, numQubits+1)]

index = -1
for i in range(0, numQubits):
    if s[i] == '1':
        index = i
        break


cx_gate = qc.create_cx(0, numQubits)
for i in range(1, numQubits):
    tmp = qc.create_cx(i, numQubits)
    cx_gate = qc.gate_gate_apply(cx_gate, tmp)

x_gate = qc.create_i(''.join(indices))
if index != -1:
    indices[index] = '1'
    x_gate = qc.create_x(''.join(indices))
    indices[index] = '0'

for i in range(index + 1, numQubits):
    if s[i] == '1':
        indices[i] = '1'
        tmp = qc.create_x(''.join(indices))
        indices[i] = '0'
        x_gate = qc.gate_gate_apply(x_gate, tmp)

cx_x_gate = qc.gate_gate_apply(x_gate, cx_gate)

start = time.time()

qc.x(numQubits)

for i in range(0, numQubits+1):
    indices[i] = "1"

h_gate = qc.create_h(''.join(indices))
qc.apply_gate(h_gate)

qc.apply_gate(cx_x_gate)

indices[numQubits] = '0'
hi_gate = qc.create_h(''.join(indices))
qc.apply_gate(hi_gate)


sampled_string = qc.measure()[:-1]
sample_count = 1

iter_count = 0

while (sampled_string != allOnes) and iter_count < numQubits:
    sample_count += 1
    sampled_string = qc.measure()[:-1]
    iter_count += 1

end = time.time()

memory = qc.size()

if iter_count >= numQubits:
    print ('Incorrect , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)
else :
    print ('Correct , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)

