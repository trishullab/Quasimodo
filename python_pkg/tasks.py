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
        "cd .. && make all && cd python_pkg/ && cp ../libquasimodo.so ."
    )
    print("* Complete")

def compile_python_module(cpp_name, extension_name):
    invoke.run(
        "g++ -g -O3 -std=c++2a -w -shared -Wall -Wextra -DHAVE_CONFIG_H -Werror -Wunused-but-set-variable -fPIC -undefined dynamic_lookup -I~/Downloads/Setup/boost_1_80_0/ "
        "`python3.9 -m pybind11 --includes` "
        "-I /Users/meghana/miniconda3/envs/qp_env/include/python3.9 -I../ "
        "{0} "
        "-o {1}`python3.9-config --extension-suffix` "
        "-L. -lquasimodo -Wl,-rpath,.".format(cpp_name, extension_name)
    )

@invoke.task(build_qcmc)
@invoke.task()
def build_pybind11(c):
    """Build the pybind11 wrapper library"""
    print_banner("Building PyBind11 Module")
    compile_python_module("quasimodo_python_wrapper.cpp", "pyquasimodo")
    print("* Complete")

@invoke.task()
def test_pybind11(c):
    """Run the script to test PyBind11"""
    print_banner("Testing PyBind11 Module")
    invoke.run("python3 pybind11_test.py", pty=True)
