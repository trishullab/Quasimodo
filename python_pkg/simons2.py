import sys
import quasimodo
import random
import time

def dot_product(s, eq):
    count = 0
    for i in range(0, len(s)):
        if s[i] == eq[i] and s[i] == '1':
            count += 1
    
    return count % 2

def check_correctness(s, equations):
    for eq in equations:
        if dot_product(s, eq) != 0:
            return False
    
    return True

def parse_string(s):
    new_s = ""
    for i in range(0, len(s), 2):
        new_s += s[i]
    return new_s

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

for i in range(0, 2 * numQubits, 2):
    qc.h(i)

for i in range(0, 2 * numQubits, 2):
    qc.cx(i, i + 1)

k = 0
for i in range(0, 2 * numQubits, 2):
    if s[i//2] == '1':
        m = 1
        for j in range(0, 2 * numQubits, 2):
            if s[j//2] == '1':
                qc.cx(k, m)
            m += 2
        break
    k += 2

for i in range(0, 2 * numQubits, 2):
    qc.h(i)

equations = []
for i in range(0, 2 * numQubits):
    sampled_string = qc.measure()
    sampled_string = parse_string(sampled_string)
    if not sampled_string in equations:
        equations.append(sampled_string)

end = time.time()

memory = qc.size()

is_correct = check_correctness(s, equations)

if is_correct:
    print ('Correct , time: ', (end - start), " iter_count: " , 0, " memory: ", memory)
else:
    print ('Incorrect , time: ', (end - start), " iter_count: " , 0, " memory: ", memory)

