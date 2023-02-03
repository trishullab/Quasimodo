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

start = time.time()

qc = quasimodo.QuantumCircuit(sys.argv[2], numQubits + 1, int(sys.argv[3]))

qc.x(numQubits)

for i in range(0, numQubits + 1):
    qc.h(i)

for i in range(0, numQubits):
    qc.cx(i, numQubits)

for i in range(0, numQubits):
    if s[i] == '1':
        qc.x(i)

for i in range(0, numQubits):
    qc.h(i)

sampled_string = qc.measure()[:-1]
sample_count = 1

iter_count = 0

while (sampled_string != allOnes) and iter_count < numQubits:
    sample_count += 1
    sampled_string = qc.measure()[:-1]
    iter_count += 1

end = time.time()

if iter_count >= numQubits:
    print ('Incorrect , time: ', (end - start), " iter_count: " , iter_count)
else :
    print ('Correct , time: ', (end - start), " iter_count: " , iter_count)

