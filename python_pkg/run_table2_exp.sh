#! /bin/bash

ulimit -s unlimited
# Compiling and running WCFLOBDD and CFLOBDD
cd ../
make clean
cd python_pkg/
invoke build-quasimodo
invoke build-pybind11
rm -rf results/
mkdir results/
echo "====== Running quantum benchmarks ====="
source run.sh
