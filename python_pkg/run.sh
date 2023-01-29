#! /bin/bash

numRuns=50
range=12
options=('CFLOBDD' 'BDD')

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=$range; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 ghz.py $numQubits $j $c >> ghz_${j}_${numQubits}.txt"
            timeout -v 15m python3 ghz.py $numQubits $j $c
        done
    done
done
