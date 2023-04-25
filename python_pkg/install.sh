#! /bin/bash

check_and_install_pkg() {
  REQUIRED_PKG=$1
  PKG_OK=$(dpkg-query -W --showformat='${Status}\n' $REQUIRED_PKG|grep "install ok installed")
  echo Checking for $REQUIRED_PKG: $PKG_OK
  if [ "" = "$PKG_OK" ]; then
    echo "No $REQUIRED_PKG. Setting up $REQUIRED_PKG."
    sudo apt-get --yes install $REQUIRED_PKG
  fi
}
sudo apt-get update
check_and_install_pkg "python3"

# Install conda
wget https://repo.anaconda.com/miniconda/Miniconda3-latest-Linux-x86_64.sh .
chmod a+x Miniconda3-latest-Linux-x86_64.sh
bash Miniconda3-latest-Linux-x86_64.sh
source ~/.bashrc
conda create -n tool python=3.9
conda activate tool

export PYTHON_INCLUDE=`python -c "from sysconfig import get_paths as gp; print(gp()[\"include\"])"`

# Install make
check_and_install_pkg "make"

# Download Boost C++ library
wget https://boostorg.jfrog.io/artifactory/main/release/1.81.0/source/boost_1_81_0.tar.gz .
tar -xvf boost_1_81_0.tar.gz
cd boost_1_81_0/
export BOOST_PATH=$(pwd)
cd ..

# Install MPFR
sudo apt-get update
sudo apt-get install libmpfr-dev

# Install pip
check_and_install_pkg "python3-pip"

# Install invoke
pip install invoke

# Install pybind11
pip install pybind11

# Install gcc and g++
sudo apt-get update
check_and_install_pkg "build-essential"

# Install autoconf
sudo apt-get update
check_and_install_pkg "autoconf"

# Install git
check_and_install_pkg "git"


# Clone repository
git clone https://github.com/trishullab/Quasimodo.git
cd Quasimodo/
git submodule update --init

# Build CUDD Library used for BDDs
cd cflobdd/cudd-complex-big/
autoreconf

# Change a few settings
sed -i 's/: ${CFLAGS="-Wall -Wextra -g -O3"}/: ${CFLAGS="-Wall -Wextra -g -O3 -fPIC"}/g' configure
sed -i 's/: ${CXXFLAGS="-Wall -Wextra -std=c++0x -g -O3"}/: ${CXXFLAGS="-Wall -Wextra -std=c++0x -g -O3 -fPIC"}/g' configure

# Run and make CUDD library
./configure
make 
cd ../..

# Building Quasimodo
cd python_pkg/
# Not required; already fixed
# sed -i 's/-I${3}/-I{3}/g' tasks.py
invoke build-quasimodo
invoke build-pybind11

