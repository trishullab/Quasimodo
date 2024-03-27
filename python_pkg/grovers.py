import sys
import quasimodo
import time
import math
import random

numQubits = int(sys.argv[1])
random.seed(int(sys.argv[3]))
s = ""
for i in range(0, numQubits):
    r = random.randint(0, 1)
    if r == 0:
        s = s + '0'
    else:
        s = s + '1'

iters = math.ceil((math.pi/4 * math.sqrt((2 ** numQubits))))

start = time.time()
qc = quasimodo.QuantumCircuit(sys.argv[2], 2 * numQubits - 1, int(sys.argv[3]))

for i in range(numQubits):
    qc.h(i)

for j in range(0, iters):
    for i in range(0, numQubits):
        if s[i] == '0':
            qc.x(i)
    
    qc.ccx(0, 1, numQubits)
    
    for i in range(2, numQubits):
        qc.ccx(i, numQubits + i - 2, numQubits + i - 1)

    qc.z(2 * numQubits - 2)

    for i in range(numQubits-1, 1, -1):
        qc.ccx(i, numQubits + i - 2, numQubits + i - 1)
    
    qc.ccx(0, 1, numQubits)

    for i in range(0, numQubits):
        if s[i] == '0':
            qc.x(i)

    for i in range(numQubits):
        qc.h(i)
        qc.x(i)
    
    qc.ccx(0, 1, numQubits)

    for i in range(2, numQubits):
        qc.ccx(i, numQubits + i - 2, numQubits + i - 1)
    
    qc.z(2 * numQubits - 2)

    for i in range(numQubits-1, 1, -1):
        qc.ccx(i, numQubits + i - 2, numQubits + i - 1)

    qc.ccx(0, 1, numQubits)

    for i in range(numQubits):
        qc.x(i)
        qc.h(i)

sampled_string = qc.measure()[0:numQubits]

sample_count = 1

iter_count = 0

while (sampled_string != s) and iter_count < numQubits:
    sample_count += 1
    sampled_string = qc.measure()[0:numQubits]
    iter_count += 1

end = time.time()

memory = qc.size()

if iter_count >= numQubits:
    print ('Incorrect , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)
else :
    print ('Correct , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)
