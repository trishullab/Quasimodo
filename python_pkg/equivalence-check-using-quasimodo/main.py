import sys
import os

# Add the parent folder to sys.path
parent_dir = os.path.abspath(os.path.join(os.getcwd(), ".."))
if parent_dir not in sys.path:
    sys.path.append(parent_dir)

import Create_quantum_circuit
import Checking_Weak_Equivalence
import quantum_circuit_2D
import random
import time
import argparse

def print_stats(qc_info):
    num_gates = len(qc_info)
    num_qubits = max(max(gate['qubits']) for gate in qc_info) + 1 if qc_info else 0
    depth = max(gate['layer'] for gate in qc_info) + 1 if qc_info else 0
    num_t_gates = sum(1 for gate in qc_info if gate['type'] == 't')
    num_h_gates = sum(1 for gate in qc_info if gate['type'] == 'h')
    num_s_gates = sum(1 for gate in qc_info if gate['type'] == 's')
    num_cx_gates = sum(1 for gate in qc_info if gate['type'] == 'cx')
    print(f"Number of gates: {num_gates}")
    print(f"Number of qubits: {num_qubits}")
    print(f"Depth of the circuit: {depth}")
    print(f"Number of T gates: {num_t_gates}")
    print(f"Number of H gates: {num_h_gates}")
    print(f"Number of S gates: {num_s_gates}")
    print(f"Number of CX gates: {num_cx_gates}")


parser = argparse.ArgumentParser(description='Generate and check quantum circuits.')
parser.add_argument('num_qubits', type=int, help='Number of qubits')
parser.add_argument('depth', type=int, help='Depth of the circuit')
parser.add_argument('cnot_density', type=float, help='CNOT density percentage')
parser.add_argument('t_density', type=float, help='T density percentage')
parser.add_argument('--seed', type=int, default=random.randint(0, 1000000), help='Random seed')
parser.add_argument('--should_be_equal', dest='should_be_equal', action='store_true', help='Make circuits equal')
parser.add_argument('--different', dest='should_be_equal', action='store_false', help='Make circuits different')
parser.set_defaults(should_be_equal=True)
parser.add_argument('--add_hadamard_layer', dest='add_hadamard_layer', action='store_true', help='Add hadamard layer to circuit')
parser.add_argument('--no_hadamard_layer', dest='add_hadamard_layer', action='store_false', help='Dont add hadamard layer to circuit')
parser.add_argument('--data_structure', type=str, choices=['CFLOBDD', 'WCFLOBDD', 'MQTDD'], help='Data structure to use: CFLOBDD or WCFLOBDD or MQTDD')
parser.add_argument('--run_sequential', dest='run_sequential', action='store_true', help='Run sequentially')
parser.add_argument('--no_run_sequential', dest='run_sequential', action='store_false', help='Dont run sequentially')
parser.set_defaults(add_hadamard_layer=False)
parser.set_defaults(run_sequential=False)

args = parser.parse_args()

num_qubits = args.num_qubits
depth = args.depth
cnot_density = args.cnot_density / 100.0
t_density = args.t_density / 100.0
seed = args.seed
should_be_equal = args.should_be_equal
add_hadamard_layer = args.add_hadamard_layer
data_structure = args.data_structure
run_sequential = args.run_sequential
random.seed(seed)

qc_1, qc_info_1 = Create_quantum_circuit.create_clifford_plus_t_circuit_based_on_density(num_qubits, depth, cnot_density=cnot_density, t_density=t_density, add_hadamard_layer=add_hadamard_layer, data_structure=data_structure)
print("Created one random circuit with depth ", depth, " and number of qubits ", num_qubits, " with seed ", seed)
print("QC1 Info: ", qc_info_1)
# print_stats(qc_info_1)

if should_be_equal:
    print("Both circuits are made equal for testing purposes." if should_be_equal else "Both circuits are made different for testing purposes.")
    start = time.time()
    are_equal, max_light_cone_size = quantum_circuit_2D.local_projection_check_if_two_circuits_are_equal(qc_info_1,qc_info_1, qc=qc_1, apply_sequential=run_sequential)
    end = time.time()
    print("Expected: The circuits are made equal for testing purposes, Actual: ", are_equal, "Time taken (s): ", end - start, "Max light cone size: ", max_light_cone_size)
else:
    qc_2, qc_info_2 = Create_quantum_circuit.create_clifford_plus_t_circuit_based_on_density(num_qubits, depth, cnot_density=cnot_density, t_density=t_density, add_hadamard_layer= add_hadamard_layer, data_structure=data_structure)
    print("Created another random circuit with depth ", depth, " and number of qubits ", num_qubits, " with seed ", seed)
    print("Both circuits are made equal for testing purposes." if should_be_equal else "Both circuits are made different for testing purposes.") 
    start = time.time()
    are_equal, max_light_cone_size = quantum_circuit_2D.local_projection_check_if_two_circuits_are_equal(qc_info_1,qc_info_2, qc=qc_1)
    end = time.time()
    print("Expected: The circuits are different for testing purposes, Actual: ", are_equal, "Time taken (s): ", end - start, "Max light cone size: ", max_light_cone_size)