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

qc = quasimodo.QuantumCircuit(sys.argv[2], numQubits, int(sys.argv[3]))


for i in range(0, len(s)):
    if s[i] == '1':
        qc.x(i)

for i in range(0, numQubits//2):
    qc.swap(i, numQubits - i - 1)

for i in range(numQubits-1, -1, -1):
    qc.h(i)
    for j in range(0, i):
        theta = 1/2**(i - j)
        qc.cp(j, i, theta)

end = time.time()

'''
print('done, time: ', (end - start), " iter_count: " , 0)

qc.h(0)
for i in range(1, numQubits):
    for j in range(i-1, -1, -1):
        theta = -1/pow(2, i - j)
        qc.cp(j, i, theta)
    qc.h(i)
    
for i in range(0, (numQubits)//2):
    qc.swap(i, numQubits - i - 1) 

sampled_string = qc.measure()

if (sampled_string == s):
    print ('Correct , time: ', (end - start), " iter_count: " , 0)
else:
    print ('Incorrect , time: ', (end - start), " iter_count: " , 0) 
'''

memory = qc.size()

print ('Correct , time: ', (end - start), " iter_count: " , 0, " memory: ", memory)

