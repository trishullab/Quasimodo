#!/bin/bash

cwd=$(pwd)

for file in $cwd/*.txt; do
	echo "running $file"
	python3 compute.py $file >> results.txt
done;
