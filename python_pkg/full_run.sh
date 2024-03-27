#! /bin/bash

numRuns=50
range=12
options=('CFLOBDD' 'BDD' 'WBDD')

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=$range; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 ghz.py $numQubits $j $c >> ghz_${j}_${numQubits}.txt"
            timeout -v 15m python3 ghz.py $numQubits $j $c >> output/ghz_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=$range; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 bv.py $numQubits $j $c >> bv_${j}_${numQubits}.txt"
            timeout -v 15m python3 bv.py $numQubits $j $c >> output/bv_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=$range; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 dj.py $numQubits $j $c >> dj_${j}_${numQubits}.txt"
            timeout -v 15m python3 dj.py $numQubits $j $c >> output/dj_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=4; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 simons.py $numQubits $j $c >> simons_${j}_${numQubits}.txt"
            timeout -v 15m python3 simons.py $numQubits $j $c >> output/simons_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=12; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 qft.py $numQubits $j $c >> qft_${j}_${numQubits}.txt"
            timeout -v 15m python3 qft.py $numQubits $j $c >> output/qft_${j}_${numQubits}.txt
        done
    done
done

for (( c=0; c<$numRuns; c++ ))
do
    for (( i=2; i<=6; i++))
    do
        numQubits=$((2 ** $i))
        for j in "${options[@]}";
        do
            echo "python3 grovers.py $numQubits $j $c >> grovers_${j}_${numQubits}.txt"
            timeout -v 15m python3 grovers.py $numQubits $j $c >> output/grovers_${j}_${numQubits}.txt
        done
    done
done


