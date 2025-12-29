from qiskit import QuantumCircuit, transpile
#from qiskit_aer.primitives import Estimator as local_estimator
from qiskit.quantum_info import random_unitary
from qiskit.circuit.library import UnitaryGate
from qiskit.circuit.library import RZGate
from qiskit.quantum_info import Operator
import numpy as np
import json
from qiskit.quantum_info import SparsePauliOp
from qiskit.quantum_info import Pauli
import os
import matplotlib.pyplot as plt
import quasimodo
import random

# from qiskit.circuit.library import RZGate
# Function to add_gate and generate random unitary circuits

# Function to add a quantum gate to a circuit and store gate information
def add_gate(gate_name, qubits, gates_info, qc=None, gate_matrix=None):
    """
    Adds a quantum gate or measurement to the circuit and stores gate information.

    Arguments:
    - gate_name (str): Name of the gate to add (e.g., 'cx', 'h', 'unitary', 'rz', 'sx', 'measure').
    - qubits (list[int]): Qubits on which the gate operates.
    - gates_info (list): A list to store information about the gates added.
    - qc (QuantumCircuit, optional): The quantum circuit. If None, a new one is created.
    - gate_matrix (optional): Matrix for unitary gate operations, used only if gate is 'unitary'.

    Returns:
    - qc (QuantumCircuit): Updated quantum circuit.
    - gates_info (list): Updated gate information list.
    """
    if qc is None:
        qc = QuantumCircuit(max(qubits) + 1, max(qubits) + 1)  # Init classical bits for measurements
    
    gate_info = {
        'qubits': qubits,
        'type': gate_name.lower()
    }
    
    # Map gate name to appropriate Qiskit functions and add to circuit
    if gate_name.lower() == 'cx':
        qc.cx(qubits[0], qubits[1])
        gate_info['matrix'] = np.array([[1, 0, 0, 0],
                                        [0, 1, 0, 0],
                                        [0, 0, 0, 1],
                                        [0, 0, 1, 0]]).tolist()
    elif gate_name.lower() == 'cz':
        qc.cz(qubits[0], qubits[1])
    elif gate_name.lower() in ['ccx', 'toffoli']:
        qc.ccx(qubits[0], qubits[1], qubits[2])
    elif gate_name.lower() == 'h':
        qc.h(qubits[0])
    elif gate_name.lower() == 'x':
        qc.x(qubits[0])
    elif gate_name.lower() == 'y':
        qc.y(qubits[0])
    elif gate_name.lower() == 'z':
        qc.z(qubits[0])
    elif gate_name.lower() == 'rz':
        if gate_matrix is None:
            raise ValueError("Rz gate requires an angle.")
        qc.rz(gate_matrix, qubits[0])
        gate_info['angle'] = gate_matrix
    elif gate_name.lower() == 'sx':
        qc.sx(qubits[0])
    elif gate_name.lower() == 's':
        qc.s(qubits[0])
    elif gate_name.lower() == 'sdg':
        qc.sdg(qubits[0])
    elif gate_name.lower() == 'unitary' and gate_matrix is not None:
        unitary_matrix = np.array(gate_matrix)
        gate = UnitaryGate(unitary_matrix)
        qc.append(gate, qubits)
        gate_info['matrix'] = unitary_matrix.tolist()
    elif gate_name.lower() == 'barrier':
        qc.barrier(qubits)
    elif gate_name.lower() == 'measure' or gate_name.lower() == 'measurement':
        for qubit in qubits:
            qc.measure(qubit, qubit)
    else:
        raise ValueError(f"Unsupported gate: {gate_name}")
    
    gates_info.append(gate_info)
    return qc, gates_info

def create_clifford_plus_t_circuit(n_qubits, depth):
    """
    Creates a random quantum circuit using unitary gates sampled from the Haar distribution.
    This version includes layer information in the gate definitions.

    Arguments:
    - n_qubits (int): Number of qubits in the circuit. Must be even and >= 2.
    - depth (int): Depth of the circuit, or the number of layers of gates.

    Returns:
    - qc (QuantumCircuit): The generated random quantum circuit.
    - gates_info (list): Information about the gates added to the circuit.
    """
    if n_qubits % 2 != 0:
        raise ValueError("Number of qubits must be even.")
    if n_qubits < 2:
        raise ValueError("Number of qubits must be at least 2.")
    
    # Initialize a quantum circuit
    qc = quasimodo.QuantumCircuit('CFLOBDD', n_qubits)
    gates_info = []


    one_qubit_gates = ['i', 'h', 's', 't']
    # weights = [0.15, 0.3, 0.15, 0.4]  # Weights for 'i', 'h', 's', 't'
    # one_qubit_gates = ['i', 'h', 'x']
    two_qubit_gates = ['cx']
    
    # Add layers of random unitary gates
    for layer in range(depth):
        type_of_gate = random.randint(0, 1)
        # if layer == 1:
        #     type_of_gate = 1
        # else:
        #     type_of_gate = 0
        if type_of_gate == 0:
            # SINGLE QUBIT GATES
            for i in range(0, n_qubits):
                # one_qubit_gate_type = random.choices(one_qubit_gates, weights=weights, k=1)[0]
                one_qubit_gate_type = random.choice(one_qubit_gates)

                # if layer == 0 and i == 1:
                #     one_qubit_gate_type = 'h'
                # elif layer == 0 and i == 2:
                #     one_qubit_gate_type = 's'
                # elif layer == 1 and i == 3:
                #     one_qubit_gate_type = 't'
                # elif layer == 2 and i == 0:
                #     one_qubit_gate_type = 'h'
                # elif layer == 2 and i == 1:
                #     one_qubit_gate_type = 't'
                # elif layer == 2 and i == 2:
                #     one_qubit_gate_type = 'h'
                # elif layer == 2 and i == 3:
                #     one_qubit_gate_type = 't'
                # else:
                #     one_qubit_gate_type = 'i'

                if one_qubit_gate_type == 'i':
                    continue  # Skip identity gates

                if one_qubit_gate_type == 'h':
                    indices = list('0' * n_qubits)
                    indices[i] = '1'
                    indices = ''.join(indices)
                    h_gate = qc.create_h(indices)
                    gate_info = {
                        'qubits': [i],
                        'type': 'h',
                        'matrix': h_gate,
                        'layer': layer  # Add layer information directly based on current loop
                    }
                    gates_info.append(gate_info)
                elif one_qubit_gate_type == 'x':
                    indices = list('0' * n_qubits)
                    indices[i] = '1'
                    indices = ''.join(indices)
                    x_gate = qc.create_x(indices)
                    gate_info = {
                        'qubits': [i],
                        'type': 'x',
                        'matrix': x_gate,
                        'layer': layer  # Add layer information directly based on current loop
                    }
                    gates_info.append(gate_info)
                elif one_qubit_gate_type == 's':
                    indices = list('0' * n_qubits)
                    indices[i] = '1'
                    indices = ''.join(indices)
                    s_gate = qc.create_s(indices, False)
                    gate_info = {
                    'qubits': [i],
                    'type': 's',
                    'matrix': s_gate,
                    'layer': layer  # Add layer information directly based on current loop
                    }
                    gates_info.append(gate_info)
                elif one_qubit_gate_type == 't':
                    indices = list('0' * n_qubits)
                    indices[i] = '1'
                    indices = ''.join(indices)
                    t_gate = qc.create_t(indices, False)
                    gate_info = {
                        'qubits': [i],
                        'type': 't',
                        'matrix': t_gate,
                        'layer': layer  # Add layer information directly based on current loop
                    }
                    gates_info.append(gate_info)
                else: 
                    raise ValueError(f"Unsupported one-qubit gate: {one_qubit_gate_type}")
        else:
            # TWO QUBIT GATES
            start_qubit = layer % 2  # Alternate starting qubit between 0 and 1
            for i in range(start_qubit, n_qubits - 1, 2):
                cx_gate = qc.create_cx(i, i+1)
                gate_info = {
                    'qubits': [i, i+1],
                    'type': 'cx',
                    'matrix': cx_gate,
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)

    return qc, gates_info

def create_clifford_plus_t_circuit_based_on_density(n_qubits, depth, cnot_density=0.5, t_density=0.3, add_hadamard_layer=False, data_structure='CFLOBDD'):
    """
    Creates a random quantum circuit using unitary gates sampled from the Haar distribution.
    This version includes layer information in the gate definitions.

    Arguments:
    - n_qubits (int): Number of qubits in the circuit. Must be even and >= 2.
    - depth (int): Depth of the circuit, or the number of layers of gates.

    Returns:
    - qc (QuantumCircuit): The generated random quantum circuit.
    - gates_info (list): Information about the gates added to the circuit.
    """
    if n_qubits % 2 != 0:
        raise ValueError("Number of qubits must be even.")
    if n_qubits < 2:
        raise ValueError("Number of qubits must be at least 2.")
    
    # Initialize a quantum circuit
    qc = quasimodo.QuantumCircuit(data_structure, n_qubits)
    gates_info = []


    assert (t_density + cnot_density <= 1)
    gates = ['t', 'cx', 'i', 'h', 's']
    probabilities = [t_density, cnot_density]
    for _ in range(2, len(gates)):
        remaining_probability = (1 - t_density - cnot_density)/(len(gates) - 2)
        probabilities.append(remaining_probability)

    layer_begin = 0
    if add_hadamard_layer:
        for qubit in range(n_qubits):
            indices = list('0' * n_qubits)
            indices[qubit] = '1'
            indices = ''.join(indices)
            h_gate = qc.create_h(indices)
            gate_info = {
                'qubits': [qubit],
                'type': 'h',
                'matrix': h_gate,
                'layer': 0  # Add layer information directly based on current loop
            }
            gates_info.append(gate_info)
            layer_begin = 1
    
    # Add layers of random unitary gates
    for layer in range(layer_begin, depth):
        qubit = 0
        while qubit < n_qubits:
            
            if qubit == n_qubits - 1:
                available_gates = ['t', 'i', 'h', 's']
                tmp_probs = [t_density] + [(1 - t_density)/(len(available_gates) - 1)] * (len(available_gates) - 1)
                gate = random.choices(available_gates, weights=tmp_probs, k=1)[0]
            else:
                gate = random.choices(gates, weights=probabilities, k=1)[0]
            
            if gate == 'i':
                qubit += 1
                continue # skip gate
            
            if gate == 'cx':
                cx_gate = qc.create_cx(qubit, qubit+1)
                gate_info = {
                    'qubits': [qubit, qubit+1],
                    'type': 'cx',
                    'matrix': cx_gate,
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
                qubit += 1
            elif gate == 'h':
                indices = list('0' * n_qubits)
                indices[qubit] = '1'
                indices = ''.join(indices)
                h_gate = qc.create_h(indices)
                gate_info = {
                    'qubits': [qubit],
                    'type': 'h',
                    'matrix': h_gate,
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            elif gate == 'x':
                indices = list('0' * n_qubits)
                indices[qubit] = '1'
                indices = ''.join(indices)
                x_gate = qc.create_x(indices)
                gate_info = {
                    'qubits': [qubit],
                    'type': 'x',
                    'matrix': x_gate,
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            elif gate == 's':
                indices = list('0' * n_qubits)
                indices[qubit] = '1'
                indices = ''.join(indices)
                s_gate = qc.create_s(indices, False)
                gate_info = {
                'qubits': [qubit],
                'type': 's',
                'matrix': s_gate,
                'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            elif gate == 't':
                indices = list('0' * n_qubits)
                indices[qubit] = '1'
                indices = ''.join(indices)
                t_gate = qc.create_t(indices, False)
                gate_info = {
                    'qubits': [qubit],
                    'type': 't',
                    'matrix': t_gate,
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            else: 
                raise ValueError(f"Unsupported one-qubit gate: {gate}")

            qubit += 1

    return qc, gates_info

def create_clifford_plus_t_circuit_based_on_density_using_numpy(n_qubits, depth, cnot_density=0.5, t_density=0.3, add_hadamard_layer=False):
    """
    Creates a random quantum circuit using unitary gates sampled from the Haar distribution.
    This version includes layer information in the gate definitions.

    Arguments:
    - n_qubits (int): Number of qubits in the circuit. Must be even and >= 2.
    - depth (int): Depth of the circuit, or the number of layers of gates.

    Returns:
    - qc (QuantumCircuit): The generated random quantum circuit.
    - gates_info (list): Information about the gates added to the circuit.
    """
    if n_qubits % 2 != 0:
        raise ValueError("Number of qubits must be even.")
    if n_qubits < 2:
        raise ValueError("Number of qubits must be at least 2.")
    
    # Initialize a quantum circuit
    qc = QuantumCircuit(n_qubits)
    gates_info = []


    assert (t_density + cnot_density <= 1)
    gates = ['t', 'cx', 'i', 'h', 's']
    probabilities = [t_density, cnot_density]
    for _ in range(2, len(gates)):
        remaining_probability = (1 - t_density - cnot_density)/(len(gates) - 2)
        probabilities.append(remaining_probability)

    layer_begin = 0
    if add_hadamard_layer:
        for qubit in range(n_qubits):
            qc.h(qubit)
            gate_info = {
                'qubits': [qubit],
                'type': 'h',
                'matrix': (1/np.sqrt(2) * np.array([[1, 1], [1, -1]])).tolist(),
                'layer': 0  # Add layer information directly based on current loop
            }
            gates_info.append(gate_info)
            layer_begin = 1
    
    # Add layers of random unitary gates
    for layer in range(layer_begin, depth):
        qubit = 0
        while qubit < n_qubits:
            if qubit == n_qubits - 1:
                available_gates = ['t', 'i', 'h', 's']
                tmp_probs = [t_density] + [(1 - t_density)/(len(available_gates) - 1)] * (len(available_gates) - 1)
                gate = random.choices(available_gates, weights=tmp_probs, k=1)[0]
            else:
                gate = random.choices(gates, weights=probabilities, k=1)[0]
            
            if gate == 'i':
                qubit += 1
                continue # skip gate
            
            if gate == 'cx':
                qc.cx(qubit, qubit+1)
                gate_info = {
                    'qubits': [qubit, qubit+1],
                    'type': 'cx',
                    'matrix': np.array([[1, 0, 0, 0],
                                        [0, 1, 0, 0],
                                        [0, 0, 0, 1],
                                        [0, 0, 1, 0]]).tolist(),
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
                qubit += 1
            elif gate == 'h':
                qc.h(qubit)
                gate_info = {
                    'qubits': [qubit],
                    'type': 'h',
                    'matrix': (1/np.sqrt(2) * np.array([[1, 1], [1, -1]])).tolist(),
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            elif gate == 'x':
                qc.x(qubit)
                gate_info = {
                    'qubits': [qubit],
                    'type': 'x',
                    'matrix': np.array([[1, 0], [0, 1]]).tolist(),
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            elif gate == 's':
                qc.s(qubit)
                gate_info = {
                    'qubits': [qubit],
                    'type': 's',
                    'matrix': np.array([[1, 0], [0, 0+1.j]]).tolist(),
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            elif gate == 't':
                qc.t(qubit)
                gate_info = {
                    'qubits': [qubit],
                    'type': 't',
                    'matrix': np.array([[1, 0], [0, np.exp(1j * np.pi / 4)]]).tolist(),
                    'layer': layer  # Add layer information directly based on current loop
                }
                gates_info.append(gate_info)
            else: 
                raise ValueError(f"Unsupported one-qubit gate: {gate}")

            qubit += 1
            # print(qubit, n_qubits, layer, gate)

    return qc, gates_info

# Function to create a random quantum circuit using Haar-random unitary gates
def create_random_haar_circuit(n_qubits, depth):
    """
    Creates a random quantum circuit using unitary gates sampled from the Haar distribution.
    This version includes layer information in the gate definitions.

    Arguments:
    - n_qubits (int): Number of qubits in the circuit. Must be even and >= 2.
    - depth (int): Depth of the circuit, or the number of layers of gates.

    Returns:
    - qc (QuantumCircuit): The generated random quantum circuit.
    - gates_info (list): Information about the gates added to the circuit.
    """
    if n_qubits % 2 != 0:
        raise ValueError("Number of qubits must be even.")
    if n_qubits < 2:
        raise ValueError("Number of qubits must be at least 2.")
    
    # Initialize a quantum circuit
    qc = QuantumCircuit(n_qubits)
    gates_info = []
    
    # Add layers of random unitary gates
    for layer in range(depth):
        start_qubit = layer % 2  # Alternate starting qubit between 0 and 1
        
        for i in range(start_qubit, n_qubits - 1, 2):
            # Generate a random 2-qubit unitary matrix
            dim = 4
            unitary_matrix = random_unitary(dim).data

            # Add the unitary gate to the circuit
            # qc, gates_info = add_gate('unitary', [i, i+1], gates_info, qc, unitary_matrix)
            qc, gates_info = add_gate('cx', [i, i+1], gates_info, qc)
            # Include the layer information
            gates_info[-1]['layer'] = layer  # Add layer information directly based on current loop

    return qc, gates_info


# Function to create a custom random quantum circuit using specified gates
def create_random_circuit(n_qubits, depth):
    """
    Creates a random quantum circuit using a custom composition of gates like Rz, 
    square-root X, and CZ gates.

    Arguments:
    - n_qubits (int): Number of qubits in the circuit. Must be even and >= 2.
    - depth (int): Depth of the circuit, or the number of layers of gates.

    Returns:
    - qc (QuantumCircuit): The generated custom random quantum circuit.
    - gates_info (list): Information about the gates added to the circuit.
    """
    if n_qubits % 2 != 0:
        raise ValueError("Number of qubits must be even.")
    if n_qubits < 2:
        raise ValueError("Number of qubits must be at least 2.")
    
    # Initialize a quantum circuit
    qc = QuantumCircuit(n_qubits)
    gates_info = []
    
    # Add layers of custom gate compositions
    for layer in range(depth):
        start_qubit = layer % 2  # Alternate starting qubit between 0 and 1
        
        for i in range(start_qubit, n_qubits - 1, 2):
            # Generate random angles for Rz gates
            theta_1 = np.random.uniform(0, np.pi)
            theta_2 = np.random.uniform(0, np.pi)
            
            # Define Rz, square-root X, and CZ gate matrices
            Rz_1 = [[np.exp(-1j * theta_1), 0], [0, np.exp(1j * theta_1)]]
            Rz_2 = [[np.exp(-1j * theta_2), 0], [0, np.exp(1j * theta_2)]]
            sqrt_x_matrix = (1/2) * np.array([[1 + 1j, 1 - 1j], [1 - 1j, 1 + 1j]])
            CZ_matrix = np.array([[1, 0, 0, 0], [0, 1, 0, 0], [0, 0, 1, 0], [0, 0, 0, -1]])
            
            # Create a custom unitary matrix by combining gates
            unitary_matrix = np.kron(sqrt_x_matrix, sqrt_x_matrix) @ CZ_matrix @ np.kron(Rz_1, Rz_2) @ np.kron(sqrt_x_matrix, sqrt_x_matrix)

            # Add the unitary gate to the circuit
            qc, gates_info = add_gate('unitary', [i, i+1], gates_info, qc, unitary_matrix)
    
    return qc, gates_info


# Function to create a one-shot random circuit with measurement in a specified basis
def create_one_shot_random_circuit(n_qubits, depth, basis):
    """
    Creates a random quantum circuit and measures it in the specified basis.

    Arguments:
    - n_qubits (int): Number of qubits in the circuit.
    - depth (int): Depth of the circuit.
    - basis (str): Measurement basis ('X' or 'Z').

    Returns:
    - qc (QuantumCircuit): The prepared quantum circuit ready for measurement.
    - qc_info (list): Information about the gates used in the circuit.
    """
    # Create the random circuit
    qc, qc_info = create_random_circuit(n_qubits, depth)
    
    # Apply the basis transformation and measure
    if basis == 'Z':
        qc.measure_all()
    elif basis == 'X':
        for i in range(n_qubits):
            qc.h(i)  # Apply Hadamard gate to switch to X basis
        qc.measure_all()
    else:
        raise ValueError(f"Unsupported basis: {basis}. Please use 'X' or 'Z'.")
    
    return qc, qc_info


# Function to add measurement gates to a circuit in the specified basis
def add_measurement(qc, basis):
    """
    Adds measurement gates to the circuit based on the specified basis.

    Arguments:
    - qc (QuantumCircuit): The quantum circuit to which measurements will be added.
    - basis (str): The measurement basis ('X' or 'Z').

    Returns:
    - qc (QuantumCircuit): The updated circuit with measurement gates.
    """
    n_qubits = qc.num_qubits
    
    # Apply the basis transformation and measurement gates
    if basis == 'Z':
        qc.measure_all()
    elif basis == 'X':
        qc.barrier()
        for i in range(n_qubits):
            qc.h(i)  # Apply Hadamard gate to switch to X basis
        qc.measure_all()
    else:
        raise ValueError(f"Unsupported basis: {basis}. Please use 'X' or 'Z'.")
    
    return qc




# Function to load a quantum circuit from a JSON file
def load_circuit_from_json(file_path, n_qubits=None):
    """
    Loads a quantum circuit from a JSON file containing gate information, including barriers.

    Arguments:
    - file_path (str): The path to the JSON file containing the circuit data.
    - n_qubits (int, optional): The number of qubits for the circuit. If not provided,
                                the number of qubits is inferred from the gates in the file.

    Returns:
    - qc (QuantumCircuit): The reconstructed quantum circuit based on the loaded gate information.
    - loaded_gates_info (list): A list containing information about the loaded gates (type, qubits, matrix if applicable).
    """
    with open(file_path, 'r') as f:
        gates_info = json.load(f, object_hook=complex_decoder)
    
    if n_qubits is None:
        max_qubit = max(max(info['qubits']) for info in gates_info)
        n_qubits = max_qubit + 1
    
    qc = QuantumCircuit(n_qubits)
    loaded_gates_info = []
    
    for gate_info in gates_info:
        gate_type = gate_info['type']
        qubits = gate_info['qubits']
        
        # Handle unitary gates (and other specific gate types)
        if gate_type == 'unitary':
            gate_matrix = gate_info.get('matrix')
            qc.unitary(Operator(np.array(gate_matrix)), qubits)
        elif gate_type == 'cx':
            qc.cx(qubits[0], qubits[1])
        elif gate_type == 'cz':
            qc.cz(qubits[0], qubits[1])
        elif gate_type == 'h':
            qc.h(qubits[0])
        elif gate_type == 'rz':
            angle = gate_info.get('angle')
            if angle is None:
                raise ValueError("Rz gate requires an angle.")
            qc.rz(angle, qubits[0])
        elif gate_type == 'sx':
            qc.sx(qubits[0])
        elif gate_type == 'barrier':  # Handle barrier
            qc.barrier(qubits)  # Apply the barrier to the specified qubits

        loaded_gates_info.append(gate_info)
    
    return qc, loaded_gates_info

def load_circuit_from_json(file_path, n_qubits=None):
    """
    Loads a quantum circuit from a JSON file containing gate information, including barriers and measurements.
    Ensures that measurements are applied after all the necessary gates.

    Arguments:
    - file_path (str): The path to the JSON file containing the circuit data.
    - n_qubits (int, optional): The number of qubits for the circuit. If not provided,
                                the number of qubits is inferred from the gates in the file.

    Returns:
    - qc (QuantumCircuit): The reconstructed quantum circuit based on the loaded gate information.
    - loaded_gates_info (list): A list containing information about the loaded gates (type, qubits, matrix if applicable).
    """
    with open(file_path, 'r') as f:
        gates_info = json.load(f, object_hook=complex_decoder)  # Load gates_info from JSON
    
    # If the number of qubits is not specified, infer it from the loaded gate data
    if n_qubits is None:
        max_qubit = max(max(info['qubits']) for info in gates_info)
        n_qubits = max_qubit + 1  # Number of qubits is one more than the largest index
    
    # Create a new quantum circuit with the determined number of qubits
    qc = QuantumCircuit(n_qubits, n_qubits)  # Classical bits needed for measurement
    loaded_gates_info = []

    # Separate measurement gates from the other gates
    measurement_info = []

    # Reconstruct the quantum circuit by adding gates based on the loaded information (skip measurements)
    for gate_info in gates_info:
        gate_type = gate_info['type']  # Get the type of gate (e.g., 'cx', 'h', 'unitary')
        qubits = gate_info['qubits']  # Get the qubits the gate acts on
        gate_matrix = gate_info.get('matrix')  # Get the matrix for unitary gates if applicable
        
        # Add the corresponding gate to the quantum circuit
        if gate_type == 'unitary':
            qc.unitary(Operator(np.array(gate_matrix)), qubits)
        elif gate_type == 'cx':
            qc.cx(qubits[0], qubits[1])
        elif gate_type == 'cz':
            qc.cz(qubits[0], qubits[1])
        elif gate_type == 'h':
            qc.h(qubits[0])
        elif gate_type == 'rz':
            angle = gate_info.get('angle')
            if angle is None:
                raise ValueError("Rz gate requires an angle.")
            qc.rz(angle, qubits[0])
        elif gate_type == 'sx':
            qc.sx(qubits[0])
        elif gate_type == 's':  # Add support for the S gate
            qc.s(qubits[0])
        elif gate_type == 'sdg':  # Add support for the S† (S-dagger) gate
            qc.sdg(qubits[0])
        elif gate_type == 'barrier':
            qc.barrier(qubits)
        elif gate_type == 'measurement':  # Collect measurement gates for later
            measurement_info.append(gate_info)  # Save measurement info to apply later

        # Append the gate information to the list for later inspection
        loaded_gates_info.append(gate_info)

    # After applying all other gates, apply the measurements
    for measure in measurement_info:
        qubits = measure['qubits']
        qc.measure(qubits[0], qubits[0])  # Measure qubit into classical bit

    return qc, loaded_gates_info
# Function to encode complex numbers for JSON serialization
def complex_encoder(obj):
    """
    Encodes complex numbers into a serializable format (dict) for JSON.

    Arguments:
    - obj (complex): The complex number to be serialized.

    Returns:
    - dict: A dictionary representation of the complex number with 'real' and 'imag' parts.

    Raises:
    - TypeError: If the object is not of type complex.
    """
    if isinstance(obj, complex):
        return {"real": obj.real, "imag": obj.imag}  # Convert complex number to dictionary with real and imaginary parts
    raise TypeError(f"Object of type {obj.__class__.__name__} is not JSON serializable")


# Function to decode complex numbers from JSON format
def complex_decoder(dct):
    """
    Decodes complex numbers from a dictionary format.

    Arguments:
    - dct (dict): The dictionary potentially containing complex number data.

    Returns:
    - complex: The complex number reconstructed from the 'real' and 'imag' parts.
    - dct (dict): The original dictionary if it doesn't contain complex number data.
    """
    if "real" in dct and "imag" in dct:
        return complex(dct["real"], dct["imag"])  # Reconstruct the complex number from 'real' and 'imag' fields
    return dct  # If no complex number data is found, return the original dictionary




def apply_perturbation_to_haar_circuit(circuit_info, n_qubits, sigma):
    """
    Apply a perturbation in the form of an Rz gate to each qubit involved in every 2-qubit gate in the Haar random circuit.
    If sigma is 0, the function returns the original circuit without any perturbation.

    Arguments:
    - circuit_info (list): The list of gate information (with matrices) from the Haar-random circuit.
    - n_qubits (int): The number of qubits in the circuit.
    - sigma (float): The standard deviation for the random angles used in the Rz perturbation.

    Returns:
    - perturbed_qc (QuantumCircuit): The new perturbed quantum circuit.
    - perturbed_gates_info (list): The updated list of gate information, including the perturbed gates.
    """
    # If sigma is 0, return the original circuit as is
    if sigma == 0:
        original_qc = QuantumCircuit(n_qubits)
        original_gates_info = []
        for gate_info in circuit_info:
            gate_type = gate_info['type']
            qubits = gate_info['qubits']
            gate_matrix = np.array(gate_info['matrix']) if gate_info.get('matrix') else None
            original_qc, original_gates_info = add_gate(gate_type, qubits, original_gates_info, original_qc, gate_matrix)
        return original_qc, original_gates_info

    # Otherwise, apply the perturbation
    perturbed_qc = QuantumCircuit(n_qubits)
    perturbed_gates_info = []

    # For each gate in the circuit info
    for gate_info in circuit_info:
        gate_type = gate_info['type']
        qubits = gate_info['qubits']
        gate_matrix = np.array(gate_info['matrix']) if gate_info.get('matrix') else None

        # Only apply the perturbation to 2-qubit unitary gates
        if gate_type == 'unitary' and len(qubits) == 2:
            # Generate random angles from a normal distribution for Rz perturbations
            theta_1 = np.random.normal(0, sigma)
            theta_2 = np.random.normal(0, sigma)

            # Create Rz gates as matrices
            rz_1_matrix = RZGate(theta_1).to_matrix()
            rz_2_matrix = RZGate(theta_2).to_matrix()

            # Create the full 4x4 Rz gate for the two-qubit system using the Kronecker product
            perturbation_matrix = np.kron(rz_1_matrix, rz_2_matrix)

            # Multiply the original 4x4 unitary gate matrix by the perturbation matrix
            perturbed_matrix = np.dot(perturbation_matrix, gate_matrix)

            # Add the perturbed gate to the quantum circuit
            perturbed_qc, perturbed_gates_info = add_gate('unitary', qubits, perturbed_gates_info, perturbed_qc, perturbed_matrix)
        else:
            # If it's not a 2-qubit unitary gate, just add it to the circuit unchanged
            perturbed_qc, perturbed_gates_info = add_gate(gate_type, qubits, perturbed_gates_info, perturbed_qc, gate_matrix)

    return perturbed_qc, perturbed_gates_info


def save_circuit_to_json(gates_info, output_file):
    """
    Save the perturbed gate information to a JSON file.

    Arguments:
    - perturbed_gates_info (list): The list containing information about the perturbed gates.
    - output_file (str): The output file path where the perturbed circuit information will be saved.
    """
    with open(output_file, 'w') as f:
        json.dump(gates_info, f, default=complex_encoder, indent=4)

def load_gates_info(file_path):
    """Load gate info from a JSON file."""
    with open(file_path, 'r') as f:
        gates_info = json.load(f, object_hook=complex_decoder)
    return gates_info


def apply_and_save_perturbation(input_file, output_file, sigma):
    """
    Load a quantum circuit from a JSON file, apply a perturbation, and save the perturbed circuit to a new JSON file.
    If sigma is 0, no perturbation is applied, and the original circuit is saved.

    Arguments:
    - input_file (str): Path to the input JSON file containing the original circuit information.
    - output_file (str): Path to the output JSON file where the perturbed circuit information will be saved.
    - sigma (float): The standard deviation for the normal distribution used to generate random angles for perturbations.
    """
    # Load the original circuit information from the JSON file
    qc, circuit_info = load_circuit_from_json(input_file)

    # Get the number of qubits in the circuit
    n_qubits = qc.num_qubits

    # Apply perturbations to the circuit or return the original if sigma is 0
    perturbed_qc, perturbed_gates_info = apply_perturbation_to_haar_circuit(circuit_info, n_qubits, sigma)

    # Save the perturbed circuit information to a new JSON file
    save_circuit_to_json(perturbed_gates_info, output_file)

    print(f"Perturbed circuit saved to {output_file}")



def extend_and_concatenate_gates_info(json_files):
    """
    Concatenates gate information from multiple JSON files and adjusts qubit indices for a larger circuit.
    Uses the `add_gate` function to handle adding a variety of gate types (h, sdg, s, cx, cz, rz, sx, unitary, and barrier).

    Parameters:
    - json_files (list): A list of paths to the JSON files containing gate information.

    Returns:
    - QuantumCircuit, extended_gates_info: The extended quantum circuit and the combined gates information.
    """
    combined_gates_info = []
    qubit_offset = 0
    max_qubits = 0

    # Iterate through each file to load gates_info and adjust qubit indices
    for file_path in json_files:
        # Load gates_info from each file
        gates_info = load_gates_info(file_path)
        
        # Find the maximum qubit index used in this circuit
        max_qubit_in_circuit = max(max(info['qubits']) for info in gates_info) + 1
        
        # Adjust the qubit indices in the current gates_info based on the qubit offset
        for gate_info in gates_info:
            adjusted_qubits = [qubit + qubit_offset for qubit in gate_info['qubits']]
            gate_info['qubits'] = adjusted_qubits
            combined_gates_info.append(gate_info)
        
        # Update the qubit offset for the next circuit
        qubit_offset += max_qubit_in_circuit
        max_qubits += max_qubit_in_circuit
    
    # Now construct the quantum circuit from the combined gates_info with adjusted qubit indices
    qc = QuantumCircuit(max_qubits)
    qc_info = []

    # Iterate over the combined gates_info and use the add_gate function to apply the gates
    for gate_info in combined_gates_info:
        gate_type = gate_info['type']
        qubits = gate_info['qubits']
        gate_matrix = gate_info.get('matrix')  # This will be None if not a unitary gate
        
        # Use add_gate to handle all types of gates
        qc, qc_info = add_gate(gate_type, qubits, qc_info, qc, gate_matrix)
    
    return qc, combined_gates_info


def load_circuit_and_add_measurements(gates_info, n_qubits, pauli_string):
    """
    Loads a quantum circuit from the gates_info and adds measurement operations based on the Pauli string.
    A barrier is applied before measurements to ensure all gates are executed before measuring.
    Uses the add_gate function to handle adding gates.

    Arguments:
    - gates_info (list): A list containing information about the gates (without measurements).
    - n_qubits (int): Number of qubits in the circuit.
    - pauli_string (str): A Pauli string (e.g., 'XZXY') indicating the basis for each qubit.

    Returns:
    - qc (QuantumCircuit): The quantum circuit with the gates loaded from gates_info and the measurements added.
    - gates_info_with_measurements (list): The updated gates_info, including the measurement operations.
    """
    # Step 1: Initialize the circuit with n_qubits and n classical bits
    qc = QuantumCircuit(n_qubits, n_qubits)
    gates_info_with_measurements = []

    # Step 2: Apply the gates from gates_info using add_gate function
    for gate_info in gates_info:
        gate_type = gate_info['type']
        qubits = gate_info['qubits']
        gate_matrix = gate_info.get('matrix')  # Use gate_matrix if it's a unitary gate
        
        # Use add_gate to apply the gate
        qc, gates_info_with_measurements = add_gate(gate_type, qubits, gates_info_with_measurements, qc, gate_matrix)

    # Step 3: Add a barrier before measurement to ensure all gates are applied
    qc, gates_info_with_measurements = add_gate('barrier', list(range(n_qubits)), gates_info_with_measurements, qc)

    # Step 4: Prepare for measurement based on the Pauli string
    for i, pauli in enumerate(pauli_string):
        if pauli == 'X':
            # Apply Hadamard gate to rotate the basis from Z to X
            qc, gates_info_with_measurements = add_gate('h', [i], gates_info_with_measurements, qc)
        elif pauli == 'Y':
            # Apply S† (S dagger) followed by Hadamard to rotate from Z to Y
            qc, gates_info_with_measurements = add_gate('sdg', [i], gates_info_with_measurements, qc)
            qc, gates_info_with_measurements = add_gate('h', [i], gates_info_with_measurements, qc)

    # Step 5: Add another barrier before applying measurements
    qc, gates_info_with_measurements = add_gate('barrier', list(range(n_qubits)), gates_info_with_measurements, qc)

    # Step 6: Add the measurement operation for each qubit based on the Pauli string
    for i in range(n_qubits):
        qc, gates_info_with_measurements = add_gate('measure', [i], gates_info_with_measurements, qc)
        gates_info_with_measurements.append({
            'type': 'measurement',
            'qubits': [i],
            'basis': pauli_string[i]
        })


    return qc, gates_info_with_measurements


def add_measurement(qc, pauli_string, n_qubits, gates_info):
    """
    Adds measurement operations to a quantum circuit and records the necessary gates in gates_info.
    Measurements are applied after all other gate operations.

    Arguments:
    - qc (QuantumCircuit): The quantum circuit to which measurement gates will be added.
    - pauli_string (str): A Pauli string (e.g., 'XZXY') indicating the basis for each qubit.
    - n_qubits (int): The number of qubits in the circuit.
    - gates_info (list): A list to store information about the gates and measurements.

    Returns:
    - qc (QuantumCircuit): The quantum circuit with the necessary gates and measurements added.
    - gates_info (list): The updated gates_info with the necessary gates and measurements added.
    """
    if len(pauli_string) != n_qubits:
        raise ValueError("The length of the Pauli string must match the number of qubits.")

    # Track measurement info to apply after gates
    measurement_info = []

    # Iterate over each qubit and collect the corresponding measurement basis and operations
    for i, pauli in enumerate(pauli_string):
        if pauli == 'X':
            # Apply a Hadamard gate to rotate the basis from Z to X
            qc.h(i)
            gates_info.append({
                'type': 'h',
                'qubits': [i],
                'basis': 'X'
            })
        elif pauli == 'Y':
            # Apply an S† (S dagger) followed by a Hadamard to rotate from Z to Y
            qc.sdg(i)
            qc.h(i)
            gates_info.append({
                'type': 'sdg',
                'qubits': [i],
                'basis': 'Y'
            })
            gates_info.append({
                'type': 'h',
                'qubits': [i],
                'basis': 'Y'
            })
        
        # Collect measurement instructions to be added later
        measurement_info.append({
            'type': 'measurement',
            'qubits': [i],
            'basis': pauli
        })

    # Add measurements after all other operations
    for measure in measurement_info:
        qubits = measure['qubits']
        qc.measure(qubits[0], qubits[0])  # Measure qubit into classical bit
        gates_info.append(measure)  # Update gates_info with the measurement

    return qc, gates_info



def load_inverse_circuit_from_gate_info(gates_info, n_qubits):
    """
    Loads the inverse circuit and its gate information based on gates_info.

    Arguments:
    - gates_info (list): List of gate information with a 'layer' key for each gate.
    - n_qubits (int): Total number of qubits in the circuit.

    Returns:
    - inverse_qc (QuantumCircuit): The inverse quantum circuit.
    - inverse_gates_info (list): Inverted gate information list, with 'layer' indices reversed.
    """
    # Determine the maximum layer in gates_info
    max_layer = max(gate['layer'] for gate in gates_info) if gates_info else -1
    inverse_gates_info = []
    inverse_qc = QuantumCircuit(n_qubits)

    # Process gates in reverse order
    for gate_info in sorted(reversed(gates_info), key=lambda x: (-x['layer'], min(x['qubits']))):
        gate_type = gate_info['type']
        qubits = gate_info['qubits']
        layer = gate_info['layer']
        gate_matrix = gate_info.get('matrix')
        inverse_layer = max_layer - layer

        # Inverse the gate matrix if it is a unitary operation
        if gate_type == 'unitary' and gate_matrix is not None:
            inverse_matrix = np.conjugate(np.transpose(gate_matrix))  # Compute inverse of the unitary matrix
            inverse_gate_info = {
                'type': 'unitary',
                'qubits': qubits,
                'layer': inverse_layer,
                'matrix': inverse_matrix.tolist()
            }
            inverse_qc.unitary(Operator(inverse_matrix), qubits)
        else:
            inverse_gate_info = {
                'type': gate_type,
                'qubits': qubits,
                'layer': inverse_layer
            }

            # Add standard gates as their own inverse
            if gate_type == 'cx':
                inverse_qc.cx(qubits[0], qubits[1])
                inverse_gate_info['matrix'] = np.array([[1, 0, 0, 0],
                                                        [0, 1, 0, 0],
                                                        [0, 0, 0, 1],
                                                        [0, 0, 1, 0]]).tolist()
            elif gate_type == 'cz':
                inverse_qc.cz(qubits[0], qubits[1])
            elif gate_type == 'h':
                inverse_qc.h(qubits[0])
                inverse_gate_info['matrix'] = np.array([[1, 1], [1, -1]]) / np.sqrt(2).tolist()
            elif gate_type == 'x':
                inverse_qc.x(qubits[0])
                inverse_gate_info['matrix'] = np.array([[0, 1], [1, 0]]).tolist()
            elif gate_type == 'y':
                inverse_qc.y(qubits[0])
            elif gate_type == 'z':
                inverse_qc.z(qubits[0])
            elif gate_type == 'sx':
                inverse_qc.sx(qubits[0])
            elif gate_type == 's':
                inverse_qc.sdg(qubits[0])
                inverse_gate_info['matrix'] = np.array([[1, 0], [0, 0-1j]]).tolist()
            elif gate_type == 't':
                inverse_qc.tdg(qubits[0])
                inverse_gate_info['matrix'] = np.array([[1, 0], [0, np.exp(-1j * np.pi / 4)]]).tolist()
            elif gate_type == 'sdg':
                inverse_qc.s(qubits[0])
            elif gate_type == 'rz':
                angle = gate_info.get('angle', 0)
                inverse_qc.rz(-angle, qubits[0])
                inverse_gate_info['angle'] = -angle
            elif gate_type == 'barrier':
                inverse_qc.barrier(qubits)
            elif gate_type == 'measure':
                inverse_qc.measure(qubits[0], qubits[0])

        inverse_gates_info.append(inverse_gate_info)

    # Sort the inverse gate information by 'layer' and 'initial_qubit'
    inverse_gates_info.sort(key=lambda gate: (gate['layer'], min(gate['qubits'])))

    return inverse_qc, inverse_gates_info

def load_inverse_circuit_from_gate_info_using_quasimodo(gates_info, n_qubits, inverse_qc=None):
    """
    Loads the inverse circuit and its gate information based on gates_info.

    Arguments:
    - gates_info (list): List of gate information with a 'layer' key for each gate.
    - n_qubits (int): Total number of qubits in the circuit.

    Returns:
    - inverse_qc (QuantumCircuit): The inverse quantum circuit.
    - inverse_gates_info (list): Inverted gate information list, with 'layer' indices reversed.
    """
    # Determine the maximum layer in gates_info
    max_layer = max(gate['layer'] for gate in gates_info) if gates_info else -1
    inverse_gates_info = []
    # inverse_qc = quasimodo.QuantumCircuit(n_qubits)

    # Process gates in reverse order
    for gate_info in sorted(reversed(gates_info), key=lambda x: (-x['layer'], min(x['qubits']))):
        gate_type = gate_info['type']
        qubits = gate_info['qubits']
        layer = gate_info['layer']
        gate_matrix = gate_info.get('matrix')
        inverse_layer = max_layer - layer

        # Inverse the gate matrix if it is a unitary operation
        if gate_type == 'unitary' and gate_matrix is not None:
            inverse_matrix = np.conjugate(np.transpose(gate_matrix))  # Compute inverse of the unitary matrix
            inverse_gate_info = {
                'type': 'unitary',
                'qubits': qubits,
                'layer': inverse_layer,
                'matrix': inverse_matrix.tolist()
            }
            inverse_qc.unitary(Operator(inverse_matrix), qubits)
        else:
            inverse_gate_info = {
                'type': gate_type,
                'qubits': qubits,
                'layer': inverse_layer
            }
            # Add standard gates as their own inverse
            if gate_type == 'cx':
                cx_gate = inverse_qc.create_cx(qubits[0], qubits[1])
                inverse_gate_info['matrix'] = cx_gate
            elif gate_type == 'cz':
                inverse_qc.cz(qubits[0], qubits[1])
            elif gate_type == 'h':
                indices = list('0' for _ in range(n_qubits))
                indices[qubits[0]] = '1'
                indices = ''.join(indices)
                h_gate = inverse_qc.create_h(indices)
                inverse_gate_info['matrix'] = h_gate
            elif gate_type == 'x':
                indices = list('0' for _ in range(n_qubits))
                indices[qubits[0]] = '1'
                indices = ''.join(indices)
                x_gate = inverse_qc.create_x(indices)
                inverse_gate_info['matrix'] = x_gate
            elif gate_type == 'y':
                inverse_qc.y(qubits[0])
            elif gate_type == 'z':
                inverse_qc.z(qubits[0])
            elif gate_type == 'sx':
                inverse_qc.sx(qubits[0])
            elif gate_type == 's':
                indices = list('0' for _ in range(n_qubits))
                indices[qubits[0]] = '1'
                indices = ''.join(indices)
                s_gate = inverse_qc.create_s(indices, True)
                inverse_gate_info['matrix'] = s_gate
            elif gate_type == 't':
                indices = list('0' for _ in range(n_qubits))
                indices[qubits[0]] = '1'
                indices = ''.join(indices)
                t_gate = inverse_qc.create_t(indices, True)
                inverse_gate_info['matrix'] = t_gate
            elif gate_type == 'sdg':
                inverse_qc.s(qubits[0])
            elif gate_type == 'rz':
                angle = gate_info.get('angle', 0)
                inverse_qc.rz(-angle, qubits[0])
                inverse_gate_info['angle'] = -angle
            elif gate_type == 'barrier':
                inverse_qc.barrier(qubits)
            elif gate_type == 'measure':
                inverse_qc.measure(qubits[0], qubits[0])

        inverse_gates_info.append(inverse_gate_info)

    # Sort the inverse gate information by 'layer' and 'initial_qubit'
    inverse_gates_info.sort(key=lambda gate: (gate['layer'], min(gate['qubits'])))

    return inverse_gates_info


def combine_circuits_info(circuit_info_U1, circuit_info_U2):
    """
    Combines the gate information of two circuits U1 and U2, adjusting layer indices
    for the second circuit to follow the first circuit's layers.

    Arguments:
    - circuit_info_U1 (list): The gate information for the first circuit (U1).
    - circuit_info_U2 (list): The gate information for the second circuit (U2).

    Returns:
    - combined_circuit_info (list): The combined gate information of the two circuits.
    """
    # Calculate the maximum layer in U1's circuit
    max_layer_U1 = max(gate['layer'] for gate in circuit_info_U1) if circuit_info_U1 else -1
    
    # Adjust the layers of U2 by shifting them after U1's layers
    adjusted_U2_info = []
    for gate_info in circuit_info_U2:
        adjusted_gate_info = gate_info.copy()
        adjusted_gate_info['layer'] += (max_layer_U1 + 1)  # Shift layer index by the max layer of U1 + 1
        adjusted_U2_info.append(adjusted_gate_info)
    
    # Combine the gate information from U1 and U2
    combined_circuit_info = circuit_info_U1 + adjusted_U2_info
    
    return combined_circuit_info