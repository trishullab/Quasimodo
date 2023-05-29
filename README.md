# Quasimodo: Symbolic Quantum Simulation Tool

## Prerequisites
1. Install make
2. Download and install boost c++ library in home directory
   ``` shell
      wget https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz .
      tar -xvf boost_1_81_0.tar.gz
   ```
   Export BOOST_PATH variable as 
   ``` shell
      export BOOST_PATH="$HOME/boost_1_81_0"
   ```
3. Install mpfr
    ``` shell
        sudo apt-get update
        sudo apt-get install libmpfr-dev
    ```
4. Install conda (if you want virtual environment, recommended)
    ``` shell
        wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh .
        chmod a+x Miniconda3-latest-Linux-x86_64.sh
        bash Miniconda3-latest-Linux-x86_64.sh
        source ~/.basrc
        conda create -n <env_name> python=3.9
        conda activate <env_name>
    ```
5. Set Python include path to PYTHON_INCLUDE variable
6. Install pip
7. Install invoke
    ``` shell
        pip install invoke
    ```
8. Install Pybind11
    ``` shell
        pip install pybind11
    ```
9. Install gcc, g++
10. Install autoconf
    ``` shell
        sudo apt-get update
        sudo apt-get install autoconf
    ```


## Installation Guide

1. Clone the repository and run a few init commands
  ``` shell
      git clone https://github.com/trishullab/Quasimodo.git
      cd Quasimodo/
      git submodule update --init
  ```
2. Build CUDD Library used for BDDs
  ``` shell
      cd cflobdd/cudd-complex-big/
      autoupdate
      autoreconf
  ```
  Edit configure to add -fPIC flag to CFLAGS and CXXFlAGS and then run the following after saving the file
  ``` shell
      ./configure
      make
      cd ../..
  ```
3. Let's build Quasimodo. (Edit file tasks.py in python_pkg to include -undefined dynamic_lookup on line 25 if running on MACOS)
  ``` shell
      cd python_pkg/
      invoke build-quasimodo
      invoke build-pybind11
   ```
   
Quasimodo is installed and can be used by importing to any python program.

- We have provided 6 benchmarks that can be used as a starting guide on how to write programs using Quasimodo library.
- File quantum_circuit.h provides an abstract class that shows all the supported list of gates and queries that can be performed using Quasimodo.

