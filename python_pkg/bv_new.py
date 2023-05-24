import sys
import quasimodo
import random
import time

# argv[1] -> numQubits
# argv[2] -> CFLOBDD/BDD
# argv[3] -> seed
numQubits = int(sys.argv[1])

random.seed(int(sys.argv[3]))

s = ""
for i in range(0, numQubits):
    r = random.randint(0, 1)
    if r == 0:
        s = s + '0'
    else:
        s = s + '1'


qc = quasimodo.QuantumCircuit(sys.argv[2], numQubits + 1, int(sys.argv[3]))

indices = ["0" for i in range(0, numQubits+1)]

index = -1
for i in range(0, numQubits):
    if s[i] == '1':
        index = i
        break

cx_gate = qc.create_i(''.join(indices))
if index != -1:
    cx_gate = qc.create_cx(index, numQubits)

for i in range(index + 1, numQubits):
    if s[i] == '1':
        tmp = qc.create_cx(i, numQubits)
        cx_gate = qc.gate_gate_apply(cx_gate, tmp)

start = time.time()

qc.x(numQubits)

for i in range(0, numQubits+1):
    indices[i] = "1"

h_gate = qc.create_h(''.join(indices))

qc.apply_gate(h_gate)


qc.apply_gate(cx_gate)


qc.apply_gate(h_gate)

end = time.time()
sampled_string = qc.measure()[:-1]
sample_count = 1
iter_count = 0

while (sampled_string != s) and iter_count < numQubits:
    sample_count += 1
    sampled_string = qc.measure()[:-1]
    iter_count += 1



memory = qc.size()

if iter_count >= numQubits:
    print ('Incorrect , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)
else :
    print ('Correct , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)

