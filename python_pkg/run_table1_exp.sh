#! /bin/bash

ulimit -s unlimited
# Compiling and running WCFLOBDD and CFLOBDD
cd ../cflobdd/CFLOBDD/
make clean
make cflobdd
rm -rf results_syn/
mkdir results_syn/
echo "====== Running WCFLOBDD on synthetic benchmarks ====="
source run_synthetic.sh
echo "====== Running CFLOBDD on synthetic benchmarks ====="
source run_syn_cfl.sh

cd ../../python_pkg/
cp ../cflobdd/CFLOBDD/results_syn/* results_syn/

# Compiling and running WCFLOBDD and CFLOBDD
g++ -g -O3 -std=c++2a synthetic_benchmarks.cpp -I../dd_package/
echo "====== Running WBDD on synthetic benchmarks ====="
source run_synthetic.sh
