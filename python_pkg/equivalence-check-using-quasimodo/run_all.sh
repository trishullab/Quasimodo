#!/bin/bash

# List of different inputs to use with run.sh
qubits=(4 8 16 32 64 128 256)
layers=(5 10 15 20 25 30)
# density_pairs=(
#     "20 10"
#     "35 25"
#     "60 20"
#     "75 15"
#     "35 35"
#     "50 20"
#     "15 15"
# )

density_pairs=(
    "20 1"
    "20 5"
    "20 10"
    "30 1"
    "30 5"
    "30 10"
    "15 15"
    "35 35"
    "50 20"
    "35 25"
)

data_structure=("CFLOBDD" "MQTDD" "WCFLOBDD")

run_equal="equal"
run_hadamard="hadamard"
run_sequential="sequential"

for ds in "${data_structure[@]}"; do
    OUTPUT_DIR="./output_dir/${ds}/equal_hadamard/${run_sequential}"
    mkdir -p "$OUTPUT_DIR"
done

for ds in "${data_structure[@]}"; do
    for qubit in "${qubits[@]}"; do
        for layer in "${layers[@]}"; do
            for input in "${density_pairs[@]}"; do
                echo "Running run.sh with input: $qubit, layer: $layer, density: $input for data structure: $ds"
                set -- $input
                ./run.sh "$qubit" "$layer" "$1" "$2" "$run_equal" "$run_hadamard" "$ds" "$run_sequential"
            done
        done
    done
done