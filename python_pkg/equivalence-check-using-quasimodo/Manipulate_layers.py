from qiskit import QuantumCircuit, transpile
from qiskit.quantum_info import random_unitary, Statevector, DensityMatrix, state_fidelity
from qiskit.circuit.library import UnitaryGate, RZGate
from qiskit.quantum_info import Operator, SparsePauliOp, Pauli
import numpy as np
import json
import os
import matplotlib.pyplot as plt

# import Computing_with_reduced_state
import Create_quantum_circuit
# import Simulate_local_observable

# Function to divide the gates of a quantum circuit into layers


def divide_circuit_into_layers_using_layer_index(gates_info):
    """
    Divides gates into layers based on the 'layer' key in gate_info and sorts gates within each layer 
    so that gates with smaller qubit indices appear first.

    Arguments:
    - gates_info (list): List of gate information, where each gate has a 'layer' key indicating its layer index.
    - n_qubits (int): Total number of qubits in the circuit (not used directly in this version).

    Returns:
    - layers (list): A list of lists, where each sublist represents a layer of gates sorted by qubit indices.
    - depth (int): The depth of the circuit, i.e., the total number of layers.
    """
    # Sort gates first by layer index, then by the smallest qubit index within each layer
    gates_info = sorted(gates_info, key=lambda gate: (gate['layer'], min(gate['qubits'])))

    layers = []
    current_layer_index = -1
    layer = []

    # Group gates by layer
    for gate in gates_info:
        gate_layer = gate['layer']
        
        # Start a new layer if the layer index changes
        if gate_layer != current_layer_index:
            if layer:  # Append the completed layer to layers
                layers.append(layer)
            layer = [gate]  # Start a new layer with the current gate
            current_layer_index = gate_layer
        else:
            layer.append(gate)  # Continue adding to the current layer
    
    # Append the last layer if it has any gates
    if layer:
        layers.append(layer)

    # The depth is the total number of unique layers
    depth = len(layers)
    return layers, depth



def extract_involving_qubits_at_each_layer_using_layer_index(gate_layers, target_qubits):
    """
    Extracts the set of qubits involved at each layer in a quantum circuit, starting from
    the target qubits and tracing backwards through each layer.

    Arguments:
    - gate_layers (list): A list of lists where each inner list contains gate information for a specific layer.
    - target_qubits (list[int]): The initial set of target qubits to track.
    - n_qubits (int): Total number of qubits in the circuit.

    Returns:
    - evolving_qubits_per_layer (list): A list of tuples with layer index and set of involved qubits.
    """
    evolving_qubits_per_layer = []
    evolving_qubits = set(target_qubits)

    # Traverse the layers in reverse order
    for layer_index in range(len(gate_layers) - 1, -1, -1):
        new_evolving_qubits = set(evolving_qubits)

        # Update the evolving qubits based on the qubits involved in the current layer's gates
        for gate in gate_layers[layer_index]:
            qubits_involved = set(gate['qubits'])

            # If any qubits in the gate overlap with the current evolving qubits, add all involved qubits
            if evolving_qubits.intersection(qubits_involved):
                new_evolving_qubits.update(qubits_involved)

        # Update the evolving qubits for this layer and store them sorted
        evolving_qubits = new_evolving_qubits
        evolving_qubits_per_layer.append((layer_index, sorted(evolving_qubits)))

    # Reverse the list to go from first layer to last layer
    evolving_qubits_per_layer.reverse()
    return evolving_qubits_per_layer

def retain_container_lists(list_of_lists):
    """
    Filters out lists that are subsets of other lists within a list of lists.

    Arguments:
    - list_of_lists (list of lists): The list containing lists of indices.

    Returns:
    - reduced_list (list of lists): A list containing only lists that are not subsets of other lists.
    """
    reduced_list = []

    for current_list in list_of_lists:
        is_container = True
        for other_list in list_of_lists:
            # Check if current_list is a subset of another list
            if current_list != other_list and set(current_list).issubset(set(other_list)):
                is_container = False
                break
        if is_container:
            reduced_list.append(current_list)

    return reduced_list


def compute_reduced_indices(circuit_info, n_qubits):
    """
    Computes the reduced set of indices (qubits) based on the forward light cone for each qubit.

    Arguments:
    - circuit_info (list): List containing gate information of the circuit.
    - n_qubits (int): The total number of qubits in the circuit.

    Returns:
    - set_reduced_indices (list of lists): Reduced list of indices for each qubit.
    """
    set_reduced_indices = []
    layers_qc, depth = divide_circuit_into_layers_using_layer_index(circuit_info)

    # Compute forward light cone for each qubit
    for i in range(n_qubits):
        forward_light_cone = extract_involving_qubits_at_each_layer_using_layer_index(layers_qc, target_qubits=[i])
        _, involving_qubits = forward_light_cone[0]

        # Append unique involving qubits to the set of reduced indices
        if involving_qubits not in set_reduced_indices:
            set_reduced_indices.append(involving_qubits)

    # Retain only unique lists that are not subsets of others
    return retain_container_lists(set_reduced_indices)


def get_reduced_circuit_from_layers(layers, n_qubits, target_qubits):
    """
    Constructs a reduced circuit from the specified layers and target qubits.

    Arguments:
    - layers (list of lists): The circuit layers, where each layer is a list of gate dictionaries.
    - n_qubits (int): The total number of qubits in the circuit.
    - target_qubits (list[int]): The qubits that are of interest.

    Returns:
    - reduced_circuit (QuantumCircuit): The quantum circuit reduced to only include gates involving relevant qubits.
    """
    depth = len(layers)
    involving_qubits_per_layer = extract_involving_qubits_at_each_layer_using_layer_index(layers, target_qubits)
    
    # Initialize the reduced circuit with the original number of qubits
    reduced_circuit = QuantumCircuit(n_qubits)

    # Add gates to the reduced circuit only if they involve relevant qubits
    for layer_index, layer_gates in enumerate(layers):
        relevant_qubits = involving_qubits_per_layer[layer_index][1]
        for gate_info in layer_gates:
            gate_qubits = gate_info['qubits']
            if any(qubit in relevant_qubits for qubit in gate_qubits):
                gate_name = gate_info['type']
                # Add the gate to the circuit
                if gate_name == 'cx':
                    reduced_circuit.cx(gate_qubits[0], gate_qubits[1])
                elif gate_name == 'cz':
                    reduced_circuit.cz(gate_qubits[0], gate_qubits[1])
                elif gate_name == 'ccx':
                    reduced_circuit.ccx(gate_qubits[0], gate_qubits[1], gate_qubits[2])
                elif gate_name == 'h':
                    reduced_circuit.h(gate_qubits[0])
                elif gate_name == 'x':
                    reduced_circuit.x(gate_qubits[0])
                elif gate_name == 'y':
                    reduced_circuit.y(gate_qubits[0])
                elif gate_name == 'z':
                    reduced_circuit.z(gate_qubits[0])
                elif gate_name == 'unitary':
                    unitary_matrix = np.array(gate_info['matrix'], dtype=np.complex128)
                    gate = UnitaryGate(unitary_matrix)
                    reduced_circuit.append(gate, gate_qubits)

    return reduced_circuit

# Example invocation and testing of the functions can be added here.


def extract_involving_qubits_in_forward_lightcone_using_layer_index(layers, target_qubits):
    """
    Extracts the set of qubits involved in each layer's forward lightcone in a quantum circuit,
    starting from the target qubits and working forward based on the layers' structure.

    Arguments:
    - layers (list): A list of lists where each inner list contains gate information dictionaries for a specific layer.
    - target_qubits (list[int]): The initial set of qubits to trace forward.

    Returns:
    - evolving_qubits_per_layer (list): A list of tuples with each layer index and the set of involved qubits.
    """
    evolving_qubits_per_layer = []
    evolving_qubits = set(target_qubits)  # Start with target qubits as a set

    # Traverse the layers in order
    for layer_index, layer_gates in enumerate(layers):
        new_evolving_qubits = set(evolving_qubits)  # Copy current evolving qubits as a set

        # Process each gate in the current layer to determine involved qubits
        for gate in layer_gates:
            gate_qubits = set(gate['qubits'])

            # If the gate acts on any evolving qubits, add all its qubits to new_evolving_qubits
            if evolving_qubits.intersection(gate_qubits):
                new_evolving_qubits.update(gate_qubits)

        # Update evolving qubits for the next layer and store the current layer's result
        evolving_qubits = new_evolving_qubits  # Ensure evolving_qubits remains a set
        evolving_qubits_per_layer.append((layer_index, sorted(evolving_qubits)))

    return evolving_qubits_per_layer

def get_reduced_circuit_from_forward_lightcone(layers, n_qubits, target_qubits):
    """
    Reduces a quantum circuit by extracting only the relevant gates and qubits based on target qubits,
    using the forward lightcone concept. The circuit retains the original qubit indices.

    Arguments:
    - layers (list): List of layers, where each layer is a list of gate information dictionaries.
    - n_qubits (int): The total number of qubits in the original circuit.
    - target_qubits (list[int]): The qubits to retain in the reduced circuit.

    Returns:
    - reduced_circuit (QuantumCircuit): The reduced quantum circuit, containing only the relevant gates and qubits.
    """
    
    # Extract the qubits involved in each layer based on the target qubits using forward lightcone
    involving_qubits_per_layer = extract_involving_qubits_in_forward_lightcone_using_layer_index(layers, target_qubits)
    
    # Initialize the reduced circuit with the specified number of qubits
    reduced_circuit = QuantumCircuit(n_qubits)

    # Iterate through the layers and map gates to the reduced circuit
    for layer_index, layer in enumerate(layers):
        # Retrieve the set of qubits involved in this layer
        involved_qubits = involving_qubits_per_layer[layer_index][1]

        for gate_info in layer:
            gate_qubits = gate_info['qubits']
            
            # Only add gates if they involve qubits in the current layer's forward lightcone
            if any(qubit in involved_qubits for qubit in gate_qubits):
                gate_name = gate_info['type']
                
                # Map gate to the reduced circuit based on its type
                if gate_name == 'cx':
                    reduced_circuit.cx(gate_qubits[0], gate_qubits[1])
                elif gate_name == 'cz':
                    reduced_circuit.cz(gate_qubits[0], gate_qubits[1])
                elif gate_name == 'ccx':
                    reduced_circuit.ccx(gate_qubits[0], gate_qubits[1], gate_qubits[2])
                elif gate_name == 'h':
                    reduced_circuit.h(gate_qubits[0])
                elif gate_name == 'x':
                    reduced_circuit.x(gate_qubits[0])
                elif gate_name == 'y':
                    reduced_circuit.y(gate_qubits[0])
                elif gate_name == 'z':
                    reduced_circuit.z(gate_qubits[0])
                elif gate_name == 'unitary':
                    unitary_matrix = np.array(gate_info['matrix'], dtype=np.complex128)
                    gate = UnitaryGate(unitary_matrix)
                    reduced_circuit.append(gate, gate_qubits)

    return reduced_circuit


def find_number_of_qubits_involved(qc_info):
    """
    Determines the number of qubits involved in a quantum circuit based on qc_info.

    Arguments:
    - qc_info (list): List of dictionaries, where each dictionary contains information about a gate,
                      including the 'qubits' key which lists the qubits it acts on.

    Returns:
    - int: The total number of qubits involved in the circuit.
    """
    max_qubit_index = -1  # Initialize to ensure it will be updated

    # Iterate through each gate info in qc_info to find the highest qubit index
    for gate_info in qc_info:
        qubits = gate_info['qubits']  # List of qubits for this gate
        max_qubit_index = max(max_qubit_index, *qubits)  # Update if a higher index is found

    # Total number of qubits is the highest index plus one
    return max_qubit_index + 1

def find_circuit_properties(qc_info):
    """
    Determines the number of qubits involved and the depth of a quantum circuit based on qc_info.

    Arguments:
    - qc_info (list): List of dictionaries, where each dictionary contains information about a gate,
                      including 'qubits' (list of qubits the gate acts on) and 'layer' (the layer in which the gate is applied).

    Returns:
    - dict: A dictionary containing the total number of qubits involved (`n_qubits`) and the circuit depth (`depth`).
    """
    max_qubit_index = -1  # Initialize to ensure it will be updated
    max_layer_index = -1  # Initialize to ensure it will be updated

    # Iterate through each gate info in qc_info
    for gate_info in qc_info:
        qubits = gate_info['qubits']  # List of qubits for this gate
        layer = gate_info.get('layer', -1)  # Layer information for this gate; defaults to -1 if missing

        # Update the maximum qubit index
        max_qubit_index = max(max_qubit_index, *qubits)
        
        # Update the maximum layer index to determine depth
        max_layer_index = max(max_layer_index, layer)

    # Total number of qubits is the highest index plus one
    n_qubits = max_qubit_index + 1
    
    # The depth is the highest layer index plus one (since layer index is zero-based)
    depth = max_layer_index + 1

    return  n_qubits, depth