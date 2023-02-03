import sys
import quasimodo
import time
import random

numQubits = int(sys.argv[1])

random.seed(int(sys.argv[3]))

allZeros = ""
allOnes = ""
for i in range(0, numQubits):
    allZeros += "0"
    allOnes += "1"

start = time.time()
qc = quasimodo.QuantumCircuit(sys.argv[2], numQubits, int(sys.argv[3]))

qc.h(0)

for i in range(1, numQubits):
    qc.cx(0, i)

sampled_string = qc.measure()
sample_count = 1

iter_count = 0


while (not (sampled_string == allZeros or sampled_string == allOnes)) and iter_count < numQubits:
    sample_count += 1
    sampled_string = qc.measure()
    iter_count += 1

end = time.time()

if iter_count >= numQubits:
    print ('Incorrect , time: ', (end - start), " iter_count: " , iter_count)
else :
    print ('Correct , time: ', (end - start), " iter_count: " , iter_count)

