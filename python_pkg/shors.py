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


def QFT(qc, numQubits, offset):
    # QFT on numQubits+1..2*numQubits+1

    for i in range(0, (numQubits + 1)//2):
        qc.swap(offset + i, offset + numQubits - i)

    for i in range(numQubits, -1, -1):
        qc.h(i + offset)
        for j in range(0, i):
            theta = 1.0/pow(2, i - j)
            qc.cp(j + offset, i + offset, theta)

def IQFT(qc, numQubits, offset):
    qc.h(offset)
    for i in range(1, numQubits + 1):
        for j in range(i-1, -1, -1):
            theta = -1/pow(2, i - j)
            qc.cp(j + offset, i + offset, theta)
        qc.h(i + offset)
    for i in range(0, (numQubits + 1)//2):
        qc.swap(offset + i, offset + numQubits - i) 

def ADDGateWithDoubleControl(qc, controller, x, a, numQubits, offset):
    for i in range(0, numQubits+1):
        theta = a/(2 ** (i))
        qc.ccp(controller, x, offset + i, theta)

def ADDGateWithSingleControl(qc, x, a, numQubits, offset):
    for i in range(0, numQubits+1):
        theta = a/(2 ** (i))
        qc.cp(x, offset + i, theta)

def ADDGate(qc, N, numQubits, offset):
    for i in range(0, numQubits+1):
        theta = N/(2 ** (i))
        qc.p(offset + i, theta)


def CADDModN(qc, controller, x, a, N, numQubits):
    ADDGateWithDoubleControl(qc, controller, x, a, numQubits, numQubits+1)
    ADDGate(qc, 2**(numQubits+1)-N, numQubits, numQubits+1)
    IQFT(qc, numQubits,  numQubits+1)
    qc.cx(numQubits + 1, 2 * numQubits + 2)
    QFT(qc, numQubits, numQubits+1)
    ADDGateWithSingleControl(qc, 2 * numQubits + 2, N, numQubits, numQubits+1)
    ADDGateWithDoubleControl(qc, controller, x, 2**(numQubits+1)-a, numQubits, numQubits+1)
    IQFT(qc, numQubits, numQubits+1)
    qc.x(numQubits + 1)
    qc.cx(numQubits + 1, 2 * numQubits + 2)
    qc.x(numQubits + 1)
    QFT(qc, numQubits, numQubits+1)
    ADDGateWithDoubleControl(qc, controller, x, a, numQubits, numQubits+1)


def CMULT(qc, controller, powOfa, N, numQubits):
    QFT(qc, numQubits, numQubits+1)
    a = 1
    for i in range(0, numQubits):
        if i == 0:
            a = powOfa
        else:
            a = (2 * a) % N
        
        CADDModN(qc, controller, numQubits - i, a, N, numQubits)
    IQFT(qc, numQubits, numQubits+1)


def gcd(a, b):
    if (a == 0):
        return b
 
    return gcd(b % a, a)

def power(x, y, M):
 
    if (y == 0):
        return 1
 
    p = power(x, y // 2, M) % M
    p = (p * p) % M
 
    if(y % 2 == 0):
        return p
    else:
        return ((x * p) % M)

def modInverse(a, N):
    g = gcd(a, N)
    if g != 1:
        exit()
    else:
        return power(a, N - 2, N)

# argv[1] -> numQubits
# argv[2] -> CFLOBDD/BDD
# argv[3] -> N
# argv[4] -> a
numQubits = int(sys.argv[1])
N = int(sys.argv[3])
a = int(sys.argv[4])

start = time.time()
f1 = f2 = 1
r = 0

qc = quasimodo.QuantumCircuit(sys.argv[2], 2 * numQubits + 3, r)

powersOfa = []

for i in range(0, 2 * numQubits):
    if i == 0:
        powersOfa.append(a % N)
    else:
        prev = powersOfa[-1]
        powersOfa.append((prev * prev) % N)

qc.h(0)
qc.x(numQubits) # n
controller = 0

qubit_map = {}
for i in range(0, 2 * numQubits + 3):
    qubit_map[i] = 'X'

sampled_string = ""
sampled_number = 0

for i in range(0, 2 * numQubits):
    powOfa = powersOfa[2 * numQubits - 1 - i]
        
    CMULT(qc, controller, powOfa, N, numQubits)

    for j in range(0, numQubits):
        qc.cswap(controller, 1 + j, 2 + j + numQubits)
    
    powOfaInv = modInverse(powOfa, N)
    CMULT(qc, controller, N - powOfaInv, N, numQubits)

    if i != 0:
        # theta = (2 ** (i + 1) - sampled_number)/sampled_number
        theta = (2 ** (i + 1) - sampled_number)/(2**(i + 1))
        qc.p(0, theta)

    qc.h(0)
    m = qc.measure_and_collapse([0])
    if m != '0':
        qc.x(0)
    
    qc.h(0)

    sampled_string = m + sampled_string
    if m == '1':
        sampled_number = 2 ** (i + 1) + sampled_number

(f1, f2) = factorize(Frac(sampled_string, N), N)
is_correct = False
if f1 != 1 or f2 != 1:
    is_correct = True

end = time.time()

memory = qc.size()

if f1 * f2 == N and is_correct:
    print ('Correct , time: ', (end - start), " iter_count: " , 0, " memory: ", memory)
else:
    print ('Incorrect , time: ', (end - start), " iter_count: " , 0, " memory: ", memory)
