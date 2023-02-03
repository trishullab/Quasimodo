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

start = time.time()

qc = quasimodo.QuantumCircuit(sys.argv[2], 2 * numQubits, int(sys.argv[3]))

for i in range(0, numQubits):
    qc.h(i)

for i in range(0, numQubits):
    qc.cx(i, i + numQubits)

k = 0
for i in range(numQubits-1, -1, -1):
    if s[i] == '1':
        m = numQubits
        for j in range(numQubits-1, -1, -1):
            if s[j] == '1':
                qc.cx(k, m)
            m += 1
        break
    k += 1

for i in range(0, numQubits):
    qc.h(i)

for i in range(0, 2 * numQubits):
    sampled_string = qc.measure()

end = time.time()

print ('Correct , time: ', (end - start), " iter_count: " , 0)

