#!/bin/bash
# Run tmp.py five times and save output to output_4_2.txt

# if [ "$#" -ne 4 ]; then
#     echo "Usage: $0 <arg1> <arg2> <arg3> <arg4>"
#     exit 1
# fi
OUTPUT_DIR="./output_dir/${7}/equal_hadamard/${8}"
mkdir -p "$OUTPUT_DIR"
# Add --should_be_equal flag if the fifth argument equals "equal"
if [ "$5" = "equal" ]; then
    EQUAL_PARAM="--should_be_equal"
else
    EQUAL_PARAM="--different"
fi

# Add --different flag if the fifth argument equals "different"
if [ "$5" = "different" ]; then
    EQUAL_PARAM="--different"
else
    EQUAL_PARAM="--should_be_equal"
fi

# Add --add_hadamard_layer flag if the fifth argument equals "different"
if [ "$6" = "hadamard" ]; then
    HADAMARD_PARAM="--add_hadamard_layer"
else
    HADAMARD_PARAM="--no_hadamard_layer"
fi

# Add --run_sequential flag if the eighth argument equals "sequential"
if [ "$8" = "sequential" ]; then
    RUN_SEQUENTIAL_PARAM="--run_sequential"
else
    RUN_SEQUENTIAL_PARAM="--no_run_sequential"
fi

DATA_STRUCTURE_FLAG="--data_structure"

OUTPUT_FILE="${OUTPUT_DIR}/output_${1}_${2}_${3}_${4}.txt"
> "$OUTPUT_FILE"  # Clear any existing content

for ((i=0; i<50; i++)); do
    seed=$((1000 + i * 100))
    timeout -v 5m python main.py $1 $2 $3 $4 --seed "$seed" "$EQUAL_PARAM" "$HADAMARD_PARAM" "$DATA_STRUCTURE_FLAG" "$7" "$RUN_SEQUENTIAL_PARAM" >> "$OUTPUT_FILE"
done