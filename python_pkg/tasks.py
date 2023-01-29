import invoke
import pathlib
import sys
import os
import shutil
import re
import glob

def print_banner(msg):
    print("==================================================")
    print("= {} ".format(msg))

@invoke.task()
def build_qcmc(c):
    """Build the shared library for the sample C++ code"""
    print_banner("Building C++ Library")
    invoke.run(
        "cd .. && make all && cd python_pkg/ && cp ../libqcmc.so ."
    )
    print("* Complete")

def compile_python_module(cpp_name, extension_name):
    invoke.run(
        "g++ -O3 -Wall -Werror -shared -std=c++2a -fPIC -undefined dynamic_lookup -I~/Downloads/Setup/boost_1_68_0/ "
        "`python3.9 -m pybind11 --includes` "
        "-I /Users/meghana/miniconda3/envs/qp_env/include/python3.9 -I../ "
        "{0} "
        "-o {1}`python3.9-config --extension-suffix` "
        "-L. -lqcmc -Wl,-rpath,.".format(cpp_name, extension_name)
    )

@invoke.task(build_qcmc)
@invoke.task()
def build_pybind11(c):
    """Build the pybind11 wrapper library"""
    print_banner("Building PyBind11 Module")
    compile_python_module("qcmc_python_wrapper.cpp", "pyqcmc")
    print("* Complete")

@invoke.task()
def test_pybind11(c):
    """Run the script to test PyBind11"""
    print_banner("Testing PyBind11 Module")
    invoke.run("python3 pybind11_test.py", pty=True)
