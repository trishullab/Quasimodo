import sys
import quasimodo
import time
import random


start = time.time()
inp_file = open(sys.argv[2], 'r')
Lines = inp_file.readlines()

numQubits = int(Lines[0].split(' ')[0])

qc = quasimodo.QuantumCircuit(sys.argv[1], numQubits)

for i in Lines[1:]:
    print (i)
    inst = i.split(' ')[1:]
    if inst[0] == 'h':
        qc.h(int(inst[1]))
    elif inst[0] == 't':
        qc.t(int(inst[1]))
    elif inst[0] == 'x_1_2':
        qc.sx(int(inst[1]))
    elif inst[0] == 'y_1_2':
        qc.sy(int(inst[1]))
    elif inst[0] == 'cz':
        qc.cz(int(inst[1]), int(inst[2]))
    elif inst[0] == 'is':
        qc.iswap(int(inst[1]), int(inst[2]))
qc.measure()
end = time.time()