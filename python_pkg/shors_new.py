import sys
import quasimodo
import time
import math
import random
from fractions import Fraction

def Frac(s, N):
    return Fraction(int(s, 2), 2**len(s)).limit_denominator(N).denominator

def factorize(r, N):
    a = math.gcd(2**(r//2)+1, N)
    if a != 1:
        return (a, N//a)
    b = math.gcd(2**(r//2)-1, N)
    if b != 1:
        return (b, N//b)
    return (1, 1)


# argv[1] -> numQubits
# argv[2] -> CFLOBDD/BDD
# argv[3] -> N
# argv[4] -> a
numQubits = int(sys.argv[1])
N = int(sys.argv[3])
a = int(sys.argv[4])
r = int(sys.argv[5])

start = time.time()
f1 = f2 = 1

qc = quasimodo.QuantumCircuit(sys.argv[2], 2 * numQubits + 4, r)

for i in range(numQubits):
    qc.h(i)

qc.x(numQubits+3)

for q in range(numQubits-1, -1, -1):
    p = 2**(numQubits - 1 - q)
    for i in range(p):
        if a in [2,13]:
            qc.cswap(q, numQubits, numQubits + 1)
            qc.cswap(q, numQubits + 1, numQubits + 2)
            qc.cswap(q, numQubits + 2, numQubits + 3)
        if a in [7,8]:
            qc.cswap(q, numQubits + 2, numQubits + 3)
            qc.cswap(q, numQubits + 1, numQubits + 2)
            qc.cswap(q, numQubits, numQubits + 1)
        if a in [4,11]:
            qc.cswap(q, numQubits + 1, numQubits + 3)
            qc.cswap(q, numQubits, numQubits + 2)
        if a in [7,11,13]:
            for j in range(4):
                qc.x(numQubits + j) 


for i in range(numQubits):
    for j in range(0, i):
        theta = -1/2**(i-j)
        qc.cp(j, i, theta)
    qc.h(i)

for i in range(numQubits//2):
    qc.swap(i, numQubits - 1 - i)

iter_count = 0
is_correct = False

while not is_correct:
    sampled_string = qc.measure()[:-4]
    (f1, f2) = factorize(Frac(sampled_string, N), N)
    if f1 != 1 and f2 != 1:
        is_correct = True
    iter_count += 1

end = time.time()

print(f1, f2)

memory = qc.size()

if f1 * f2 == N and is_correct:
    print ('Correct , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)
else:
    print ('Incorrect , time: ', (end - start), " iter_count: " , iter_count, " memory: ", memory)
