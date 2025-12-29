
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
import Create_quantum_circuit
import local_projection_computation
import Manipulate_layers

import time
import matplotlib as mpl
mpl.rcParams['pdf.fonttype'] = 42   # use TrueType in PDFs (no Type 3)
mpl.rcParams['ps.fonttype']  = 42
mpl.rcParams['svg.fonttype'] = 'none'  # keep text as text (avoid outlines)

from qiskit_aer import AerSimulator
from qiskit.quantum_info import Statevector

def map_2d_to_1d(x, y, L):
    """Map 2D coordinates (x, y) to 1D index."""
    return x * L + y
def map_1d_to_2d(index, L):
    """Map 1D index to 2D coordinates (x, y)."""
    x = index // L
    y = index % L
    return x, y

def create_2d_circuit(L, depth):
    if L % 2 != 0:
        raise ValueError("L must be an even number for 2D circuit construction.")
    
    gates_info = []
    qc = QuantumCircuit(L * L)

    for layer in range(depth):
        if layer % 4 == 0:
            for i in range(L):
                for j in range(L//2):
                    unitary_matrix = random_unitary(4).data
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(i, 2*j, L), map_2d_to_1d(i, 2*j+1, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer
        elif layer % 4 == 1:
            for i in range(L//2):
                for j in range(L):
                    unitary_matrix = random_unitary(4).data
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(2*i, j, L), map_2d_to_1d(2*i+1, j, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer
        elif layer % 4 == 2:
            for i in range(L):
                for j in range(L//2-1):
                    unitary_matrix = random_unitary(4).data
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(i, 2*j+1, L), map_2d_to_1d(i, 2*j+2, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer          
        elif layer % 4 == 3:
            for i in range(L//2-1):
                for j in range(L):
                    unitary_matrix = random_unitary(4).data
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(2*i+1, j, L), map_2d_to_1d(2*i+2, j, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer  
    return qc, gates_info

def create_2d_circuit_identity(L, depth):
    if L % 2 != 0:
        raise ValueError("L must be an even number for 2D circuit construction.")
    
    gates_info = []
    qc = QuantumCircuit(L * L)

    for layer in range(depth):
        if layer % 4 == 0:
            for i in range(L):
                for j in range(L//2):
                    unitary_matrix = np.eye(4)
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(i, 2*j, L), map_2d_to_1d(i, 2*j+1, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer
        elif layer % 4 == 1:
            for i in range(L//2):
                for j in range(L):
                    unitary_matrix = np.eye(4)#random_unitary(4).data
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(2*i, j, L), map_2d_to_1d(2*i+1, j, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer
        elif layer % 4 == 2:
            for i in range(L):
                for j in range(L//2-1):
                    unitary_matrix = np.eye(4)#random_unitary(4).data
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(i, 2*j+1, L), map_2d_to_1d(i, 2*j+2, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer          
        elif layer % 4 == 3:
            for i in range(L//2-1):
                for j in range(L):
                    unitary_matrix = np.eye(4)#random_unitary(4).data
                    qc, gates_info = Create_quantum_circuit.add_gate(
                        'unitary',
                        [map_2d_to_1d(2*i+1, j, L), map_2d_to_1d(2*i+2, j, L)],
                        gates_info,
                        qc,
                        unitary_matrix
                    )
                    gates_info[-1]['layer'] = layer  
    return qc, gates_info

def visualize_2d_circuit_layout(gates_info, L, depth):
    fig, axes = plt.subplots(1, depth, figsize=(4 * depth, 4))
    if depth == 1:
        axes = [axes]

    for layer in range(depth):
        ax = axes[layer]
        ax.set_title(f'Layer {layer}')
        ax.set_xlim(-0.5, L - 0.5)
        ax.set_ylim(-0.5, L - 0.5)
        ax.set_xticks(range(L))
        ax.set_yticks(range(L))
        ax.set_aspect('equal')
        ax.grid(True)

        for gate in gates_info:
            if gate['layer'] == layer:
                q1, q2 = gate['qubits']
                x1, y1 = map_1d_to_2d(q1, L)
                x2, y2 = map_1d_to_2d(q2, L)
                ax.plot([y1, y2], [x1, x2], 'ro-')

    plt.tight_layout()
    plt.savefig("circuit-2D-structure.pdf", dpi=400)
    plt.show()

def visualize_2d_circuit_layout(gates_info, L, depth):
    fig, axes = plt.subplots(1, depth, figsize=(4 * depth, 4))
    if depth == 1:
        axes = [axes]

    for layer in range(depth):
        ax = axes[layer]
        ax.set_title(f'Layer {layer}', fontsize=16)
        ax.set_xlim(-0.5, L - 0.5)
        ax.set_ylim(-0.5, L - 0.5)
        ax.set_xticks(range(L))
        ax.set_yticks(range(L))
        ax.set_xticklabels(range(L), fontsize=16)
        ax.set_yticklabels(range(L), fontsize=16)
        ax.set_aspect('equal')
        ax.grid(True)

        for gate in gates_info:
            if gate['layer'] == layer:
                q1, q2 = gate['qubits']
                x1, y1 = map_1d_to_2d(q1, L)
                x2, y2 = map_1d_to_2d(q2, L)
                ax.plot([y1, y2], [x1, x2], 'ro-')
    plt.savefig("circuit-2D-structure.pdf",dpi=400)
    plt.tight_layout()
    plt.show()



def light_cone_2d_circuit(gates_info, L, depth):
    """
    Show the light cone of the qubits in the 2D circuit based on the gates_info.
    """
    light_cones = np.zeros((L*L,depth, L*L), dtype=int)   # L*L qubits, depth, L*L light cone states    
    # light_cones[q, d, k] = 1 means: qubit `q` is influenced by qubit `k` at layer `d`
    for layer in range(depth):
        if layer != 0:
            light_cones[:,layer,:] = light_cones[:,layer-1,:].copy()
        # Initialize the light cone for the current layer
        for gate in gates_info:
            if gate['layer'] == layer:
                q1, q2 = gate['qubits']
                x1, y1 = map_1d_to_2d(q1, L)
                x2, y2 = map_1d_to_2d(q2, L)
                if layer == 0:
                    light_cones[q1, layer, q1] = 1
                    light_cones[q1, layer, q2] = 1
                    light_cones[q2, layer, q1] = 1
                    light_cones[q2, layer, q2] = 1
                else:
                    for qubit in range(L*L):
                        if light_cones[qubit,layer-1, q1] == 1 or light_cones[qubit, layer-1, q2] == 1:
                            light_cones[qubit, layer, q1] = 1
                            light_cones[qubit, layer, q2] = 1
                # Update the light cone for the qubits involved in the gate
    return light_cones


def light_cone_arbitrary_connectivity_circuit(gates_info, N, depth):
    """
    Show the light cone of the qubits in the 2D circuit based on the gates_info.
    """
    light_cones = np.zeros((N, depth, N), dtype=int)   # N qubits, depth, N light cone states
    # light_cones[q, d, k] = 1 means: qubit `q` is influenced by qubit `k` at layer `d`

    for q in range(N):
        light_cones[q, 0, q] = 1  # Each qubit influences itself at layer 0
    for layer in range(0, depth):
        if layer != 0:
            light_cones[:,layer,:] = light_cones[:,layer-1,:].copy()
        # Initialize the light cone for the current layer
        for gate in gates_info:
            if gate['layer'] == layer:
                if len(gate['qubits']) == 2:
                    q1, q2 = gate['qubits']
                    light_cones[q1, layer, q1] = 1
                    light_cones[q1, layer, q2] = 1
                    light_cones[q2, layer, q1] = 1
                    light_cones[q2, layer, q2] = 1
                    if layer != 0:
                        for qubit in range(N):
                            if light_cones[qubit,layer-1, q1] == 1 or light_cones[qubit, layer-1, q2] == 1:
                                light_cones[qubit, layer, q1] = 1
                                light_cones[qubit, layer, q2] = 1
                # elif len(gate['qubits']) == 1:
                #     q1 = gate['qubits'][0]
                #     light_cones[q1, layer, q1] = 1
                #     if layer != 0:
                #         for qubit in range(N):
                #             if light_cones[qubit,layer-1, q1] == 1:
                #                 light_cones[qubit, layer, q1] = 1
                # Update the light cone for the qubits involved in the gate
    return light_cones


def reduce_light_cone(light_cones, N):
    """
    Reduce the light cone array by removing duplicate cones across qubits.

    Arguments:
    - light_cones: numpy array of shape (N, depth, N)
    - L: lattice size (L x L)

    Returns:
    - reduced_light_cones: numpy array of unique light cones, shape (num_unique, depth, N)
    """
    N, D, _ = light_cones.shape  # N qubits, depth, N light cone states

    # Reshape each light cone to a flat vector of length D * N
    flattened = light_cones.reshape(N, -1)

    # Use numpy's unique to efficiently remove duplicates
    unique_flattened = np.unique(flattened, axis=0)

    # Reshape back to 3D: (num_unique, depth, N)
    reduced_light_cones = unique_flattened.reshape(-1, D, N)

    return reduced_light_cones



def reduce_light_cone_dictionary(light_cones):
    """
    Reduce light cones by removing duplicates and grouping qubits with identical cones.

    Returns:
    - grouped_cones: list of dicts, each with:
        - 'qubits': list of qubit indices
        - 'light_cone': numpy array of shape (depth, N)
    """
    N, D, _ = light_cones.shape
    flattened = light_cones.reshape(N, -1)

    cone_map = {}
    grouped_cones = []

    for qubit in range(N):
        cone_key = tuple(flattened[qubit])  # hashable representation
        if cone_key not in cone_map:
            # First time seeing this light cone → create new group
            cone_map[cone_key] = len(grouped_cones)
            grouped_cones.append({
                'qubits': [qubit],
                'light_cone': light_cones[qubit].copy(),
                'local_projection': -1
            })
        else:
            # Add qubit to existing group
            group_id = cone_map[cone_key]
            grouped_cones[group_id]['qubits'].append(qubit)
    # print(cone_map)

    return grouped_cones

    
def plot_light_cone_matrix(light_cones, layer, L):
    # import matplotlib.pyplot as plt
    fig, ax = plt.subplots(figsize=(6, 6))
    # ax.grid(True)
    ax.set_ylim(-1, L*L)
    ax.set_xlim(-1, L*L)
    ax.set_xticks(range(0, L*L, 1))
    ax.set_yticks(range(0, L*L, 1))
    # ax.set_aspect('equal')
    # Create a grid for the light cone matrix
    # light_cones is a 3D numpy array of shape (L*L, depth, L*L)
    # Plot the light cone matrix for the specified layer
    # The light cone matrix is a 2D slice of the 3D array at the specified layer
    # The x-axis represents the influencing qubit, and the y-axis represents the affected qubit
    ax.imshow(light_cones[:, layer, :], cmap='Greys')
    ax.set_title(f"Light Cone Matrix at Layer {layer}")
    ax.set_xlabel("Influencing Qubit")
    ax.set_ylabel("Affected Qubit")
    plt.show()

def plot_light_cone_points(light_cones, layer, L):
    """
    Plot the light cone as a set of discrete points (not filled squares) for a given layer.

    Each point (i, j) represents that qubit `i` is influenced by qubit `j` at this layer.
    """
    fig, ax = plt.subplots(figsize=(8, 8))

    # Extract the binary matrix for the given layer
    data = light_cones[:, layer, :]  # Shape: (L*L, L*L)

    # Get coordinates of entries with value 1
    rows, cols = np.where(data == 1)

    # Scatter plot for all '1' entries
    ax.scatter(cols, rows, c='black', s=10)  # (x=columns, y=rows)

    # Formatting
    ax.set_title(f"Light Cone Points at Layer {layer}")
    ax.set_xlabel("Influencing Qubit Index")
    ax.set_ylabel("Affected Qubit Index")
    ax.set_xticks(range(0, L*L, max(1, L)))
    ax.set_yticks(range(0, L*L, max(1, L)))
    ax.set_xlim(-1, L*L)
    ax.set_ylim(-1, L*L)
    ax.set_aspect('equal')
    ax.grid(True, color='lightgray', linewidth=0.5)
    plt.tight_layout()
    plt.show()



def get_subset_indices(subset, big_set):
    return [big_set.index(item) for item in subset]

def compute_local_projection_2d_circuit(N, depth, gates_info):
    """
    Compute local projection matrices for groups of qubits sharing the same light cone
    in a 2D circuit with arbitrary connectivity.

    Parameters:
    - N: Total number of qubits
    - depth: Circuit depth
    - gates_info: List of gates with associated qubit indices and matrices

    Returns:
    - A list of reduced light cone groups with updated 'local_projection_matrix' for each group
    """
    def get_subset_indices(subset, big_set):
        return [big_set.index(item) for item in subset]
    # Step 1: Compute the light cone for each qubit
    light_cones = light_cone_arbitrary_connectivity_circuit(gates_info, N, depth)

    # Step 2: Group qubits that share the same light cone structure
    reduce_light_cones = reduce_light_cone_dictionary(light_cones)

    # Step 3: Organize the gates into circuit layers
    layers, depth = Manipulate_layers.divide_circuit_into_layers_using_layer_index(gates_info)

    # print("Layers divided:", layers)

    # Step 4: Process each group of qubits with identical light cone
    for group_index, light_cone_group in enumerate(reduce_light_cones):
        # print("----------------")
        # print("Light cone group:", group_index)
        # print("Qubits in light cone group:", light_cone_group['qubits'])

        # Step 5: Iterate through each layer in depth
        for i in range(depth):
            # print("Layer", i)
            cone = light_cone_group['light_cone']
            # print("check_point_1")
            if i == 0:
                # First layer: initialize projection matrix to |00...0⟩⟨00...0|
                # print(cone[i])
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                previous_local_projection_matrix = np.zeros(
                    (2**len(qubits_in_current_layer), 2**len(qubits_in_current_layer)), dtype=complex)
                previous_local_projection_matrix[0, 0] = 1  # |00⟩⟨00|

                # print("Qubits in current layer:", qubits_in_current_layer)

                # Apply the first valid gate in the layer (for efficiency)
                for gates in layers[i]:
                    # print("Gate in layer", i, ":", gates['qubits'])
                    # print("check_point_2")

                    # Sanity check: gate must be fully within the current light cone
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        gate_index_in_current_layer = get_subset_indices(gates['qubits'], qubits_in_current_layer)
                        # print("check_point_3")
                        gate_full_matrix = embed_unitary_matrix(
                            operator_matrix=np.array(gates['matrix']),
                            target_qubits=gate_index_in_current_layer,
                            n_qubits=len(qubits_in_current_layer))
                        # print("current unitary: ", gate_full_matrix)
                        # print("dsdsdd")
                        # print("original gate: ", gates['matrix'])
                        break  # Only one gate applied for now

                # Update the projection matrix
                current_local_projection_matrix = gate_full_matrix @ previous_local_projection_matrix @ np.conjugate(np.transpose(gate_full_matrix))

            else:
                # print(cone[i])
                # For subsequent layers, update light cone and embed previous projection
                qubit_in_previous_layer = qubits_in_current_layer.copy()
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                index_in_current_layer = get_subset_indices(qubit_in_previous_layer, qubits_in_current_layer)
                # print("check_point_2")
                # Embed previous projection matrix into new (larger) space
                previous_local_projection_matrix = embed_unitary_matrix(
                    current_local_projection_matrix, index_in_current_layer, len(qubits_in_current_layer))

                # print("Qubits in previous layer:", qubit_in_previous_layer)
                # print("Qubits in current layer:", qubits_in_current_layer)
                # print("Index in current layer:", index_in_current_layer)

                count = 0  # Count valid gates in the current light cone
                for gates in layers[i]:
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        count += 1
                        # print("Gate in layer", i, ":", gates['qubits'])

                        if count == 1:
                            # Initialize unitary with the first gate
                            current_embedded_qubits = sorted(set(gates['qubits']))
                            current_unitary = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))
                        else:
                            # Merge and embed multiple gates into one operator
                            previous_embedded_qubits = current_embedded_qubits.copy()
                            current_embedded_qubits.extend(gates['qubits'])
                            current_embedded_qubits = sorted(set(current_embedded_qubits))

                            current_unitary = embed_unitary_matrix(
                                operator_matrix=current_unitary,
                                target_qubits=get_subset_indices(previous_embedded_qubits, current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            gate_full_matrix = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            current_unitary = gate_full_matrix @ current_unitary
                        # print("current unitary: ", current_unitary)
                        # print("current_embbeded:", current_embedded_qubits)

                # print("count:", count)

                # Embed the composed unitary into the full light cone
                current_unitary = embed_unitary_matrix(
                    operator_matrix=current_unitary,
                    target_qubits=get_subset_indices(current_embedded_qubits, qubits_in_current_layer),
                    n_qubits=len(qubits_in_current_layer))
                # print("current unitary: ", current_unitary)
                # print("Final embedding for this layer:", current_embedded_qubits, qubits_in_current_layer)

                # Update the local projection matrix
                # print("Previous:", previous_local_projection_matrix)
                # print("check_point_3")
                current_local_projection_matrix = current_unitary @ previous_local_projection_matrix @ np.conjugate(np.transpose(current_unitary))

        # Assign the resulting matrix back to the group
        light_cone_group['local_projection'] = current_local_projection_matrix

    return reduce_light_cones


def compute_local_projection_2d_circuit_debug(N, depth, gates_info):
    """
    Compute local projection matrices for groups of qubits sharing the same light cone
    in a 2D circuit with arbitrary connectivity, with timing checkpoints.
    """
    def get_subset_indices(subset, big_set):
        return [big_set.index(item) for item in subset]

    # Timing checkpoint 1
    t0 = time.perf_counter()
    light_cones = light_cone_arbitrary_connectivity_circuit(gates_info, N, depth)
    t1 = time.perf_counter()
    print(f"[Time] Light cone computation: {t1 - t0:.4f} seconds")

    reduce_light_cones = reduce_light_cone_dictionary(light_cones)
    t2 = time.perf_counter()
    print(f"[Time] Reduce light cones: {t2 - t1:.4f} seconds")

    layers, depth = Manipulate_layers.divide_circuit_into_layers_using_layer_index(gates_info)
    t3 = time.perf_counter()
    print(f"[Time] Divide into layers: {t3 - t2:.4f} seconds")

    for group_index, light_cone_group in enumerate(reduce_light_cones):
        print(f"\n[Group {group_index}] Processing {len(light_cone_group['qubits'])} qubits")

        for i in range(depth):
            layer_start = time.perf_counter()
            print(f"  [Layer {i}]")

            cone = light_cone_group['light_cone']
            if i == 0:
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                print("qubit:", light_cone_group["qubits"],"->",qubits_in_current_layer)
                previous_local_projection_matrix = np.zeros(
                    (2**len(qubits_in_current_layer), 2**len(qubits_in_current_layer)), dtype=complex)
                previous_local_projection_matrix[0, 0] = 1  # |00⟩⟨00|

                for gates in layers[i]:
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("    Error: gate intersects but is not fully inside the cone")
                        return False
                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        gate_index_in_current_layer = get_subset_indices(gates['qubits'], qubits_in_current_layer)
                        gate_full_matrix = embed_unitary_matrix(
                            operator_matrix=np.array(gates['matrix']),
                            target_qubits=gate_index_in_current_layer,
                            n_qubits=len(qubits_in_current_layer))
                        break

                current_local_projection_matrix = (
                    gate_full_matrix @ previous_local_projection_matrix @ np.conjugate(gate_full_matrix.T)
                )
            else:
                qubit_in_previous_layer = qubits_in_current_layer.copy()
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                print("qubit:", light_cone_group["qubits"],"->",qubits_in_current_layer)
                index_in_current_layer = get_subset_indices(qubit_in_previous_layer, qubits_in_current_layer)

                previous_local_projection_matrix = embed_unitary_matrix(
                    current_local_projection_matrix, index_in_current_layer, len(qubits_in_current_layer))

                count = 0
                for gates in layers[i]:
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("    Error: gate intersects but is not fully inside the cone")
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        count += 1
                        if count == 1:
                            current_embedded_qubits = sorted(set(gates['qubits']))
                            current_unitary = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))
                        else:
                            previous_embedded_qubits = current_embedded_qubits.copy()
                            current_embedded_qubits.extend(gates['qubits'])
                            current_embedded_qubits = sorted(set(current_embedded_qubits))

                            current_unitary = embed_unitary_matrix(
                                operator_matrix=current_unitary,
                                target_qubits=get_subset_indices(previous_embedded_qubits, current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            gate_full_matrix = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            current_unitary = gate_full_matrix @ current_unitary

                current_unitary = embed_unitary_matrix(
                    operator_matrix=current_unitary,
                    target_qubits=get_subset_indices(current_embedded_qubits, qubits_in_current_layer),
                    n_qubits=len(qubits_in_current_layer))

                current_local_projection_matrix = (
                    current_unitary @ previous_local_projection_matrix @ np.conjugate(current_unitary.T)
                )

            layer_end = time.perf_counter()
            print(f"    [Time] Layer {i} took {layer_end - layer_start:.4f} seconds")

        light_cone_group['local_projection'] = current_local_projection_matrix

    t_end = time.perf_counter()
    print(f"\n[Total Time] compute_local_projection_2d_circuit = {t_end - t0:.4f} seconds")

    return reduce_light_cones
# def plot_light_cone_2d(light_cones, layer, target_qubit, L):
#     """
#     Plot the 2D light cone of a target qubit at a given layer.

#     Arguments:
#     - light_cones: numpy array of shape (L*L, depth, L*L)
#     - layer: layer index (int)
#     - target_qubit: index of the target qubit (int)
#     - L: lattice size (int)
#     """
#     influence_map = light_cones[target_qubit, layer, :].reshape(L, L)

#     fig, ax = plt.subplots()
#     im = ax.imshow(influence_map, cmap='Greys', origin='upper')

#     x, y = map_1d_to_2d(target_qubit, L)
#     ax.plot(y, x, 'ro')  # Highlight the target qubit in red
#     ax.set_title(f"2D Light Cone of Qubit {target_qubit} at Layer {layer}")
#     ax.set_xticks(range(L))
#     ax.set_yticks(range(L))
#     ax.set_xlabel("Column")
#     ax.set_ylabel("Row")
#     ax.grid(True)
#     plt.colorbar(im, ax=ax, label='Influence (0 or 1)')
#     plt.show()


# def plot_light_cone_matrix(light_cones, layer, L):
#     """
#     Plot the full light cone matrix with correctly aligned pixel squares.
#     """
#     data = light_cones[:, layer, :]
#     fig, ax = plt.subplots(figsize=(8, 8))

#     im = ax.imshow(data, 
#                    cmap='Greys', 
#                    interpolation='none',
#                    origin='upper',
#                    extent=[-0.5, L*L - 0.5, L*L - 0.5, -0.5])

#     ax.set_title(f"Light Cone Matrix at Layer {layer}")
#     ax.set_xlabel("Influencing Qubit Index")
#     ax.set_ylabel("Affected Qubit Index")
#     ax.set_xticks(range(0, L*L, L))
#     ax.set_yticks(range(0, L*L, L))
#     ax.grid(True, color='lightgray', linewidth=0.5)

#     plt.colorbar(im, ax=ax, label='Influence (0 or 1)')
#     plt.tight_layout()
#     plt.show()

def state_vector_simulation(qc, backend_name='aer_simulator'):
    """
    Simulate the statevector of a quantum circuit using Qiskit's Aer simulator.

    Parameters:
    - qc (QuantumCircuit): The quantum circuit to simulate.
    - backend_name (str): Backend name, typically 'aer_simulator' or 'statevector_simulator'.

    Returns:
    - statevector (Statevector): The final statevector of the quantum circuit.
    """
    simulator = AerSimulator(method='statevector')
    transpiled_circuit = transpile(qc, simulator)

    # Add a save_statevector instruction at the end of the transpiled circuit
    transpiled_circuit.save_statevector()

    # Run the circuit on the simulator and obtain the result (statevector)
    result = simulator.run(transpiled_circuit).result()
    statevector = result.get_statevector()

    # Ensure the circuit is set to use statevector simulation

    return statevector


def embed_unitary_matrix(operator_matrix, target_qubits, n_qubits):
    # print("embedded:", local_projection_computation.embed_unitary_matrix(operator_matrix,target_qubits,n_qubits))
    return local_projection_computation.embed_unitary_matrix(operator_matrix,target_qubits,n_qubits)

    # if not isinstance(operator_matrix, np.ndarray):
    #     raise TypeError("operator_matrix must be a numpy array")
    
    # gate_size = int(np.log2(operator_matrix.shape[0]))
    # if len(target_qubits) != gate_size:
    #     raise ValueError("Mismatch between operator dimension and number of target qubits")

    # target_qubits = sorted(target_qubits)

    # def get_binary_array(num, width):
    #     return [int(x) for x in format(num, f'0{width}b')]

    # dim = 2 ** n_qubits
    # full_matrix = np.zeros((dim, dim), dtype=complex)

    # for i in range(dim):
    #     input_state = get_binary_array(i, n_qubits)
    #     gate_input_idx = 0
    #     for target in target_qubits:
    #         gate_input_idx = (gate_input_idx << 1) | input_state[target]

    #     gate_output = operator_matrix[gate_input_idx]

    #     for gate_output_idx in range(len(gate_output)):
    #         if abs(gate_output[gate_output_idx]) > 1e-16:
    #             output_state = input_state.copy()
    #             gate_bits = get_binary_array(gate_output_idx, len(target_qubits))
    #             for target_idx, target in enumerate(target_qubits):
    #                 output_state[target] = gate_bits[target_idx]

    #             output_idx = int(''.join(map(str, output_state)), 2)
    #             full_matrix[output_idx, i] = gate_output[gate_output_idx]

    # return full_matrix
    


def local_projection_check_if_two_circuits_are_equal(qc_info_1, qc_info_2, tolerance=1e-10, qc=None, apply_sequential=False):
    """
    Check if two quantum circuits are equivalent using local projection matrices.
    
    This function works by:
    1. Computing the inverse of the second circuit.
    2. Combining the first circuit with the inverse of the second to form a "difference circuit."
    3. Computing local projections of the difference circuit.
    4. Verifying that the resulting projections all preserve the |0...0⟩ state.

    Parameters:
    - qc_info_1: List of gate information from circuit 1
    - qc_info_2: List of gate information from circuit 2
    - tolerance: Tolerance for comparing eigenvector preservation (default = 1e-15)

    Returns:
    - True if circuits are locally equivalent
    - False otherwise
    """

    # Step 1: Get circuit size and depth
    print("Step 1: Get circuit size and depth")
    n_qubits_1, depth_1 = Manipulate_layers.find_circuit_properties(qc_info_1)
    n_qubits_2, depth_2 = Manipulate_layers.find_circuit_properties(qc_info_2)

    # Optional check for same circuit structure (commented out in case inverse makes them match)
    # if n_qubits_1 != n_qubits_2 or depth_1 != depth_2:
    #     print('Two circuits have different qubit counts or depths')
    #     return False

    # Step 2: Invert the second circuit and concatenate it with the first
    print("Step 2: Invert the second circuit and concatenate it with the first")
    inverse_qc_info_2 = Create_quantum_circuit.load_inverse_circuit_from_gate_info_using_quasimodo(
        qc_info_2, n_qubits=n_qubits_1, inverse_qc=qc)
    Identity_circuit_info = Create_quantum_circuit.combine_circuits_info(
        qc_info_1, inverse_qc_info_2)



    """
    Compute local projection matrices for groups of qubits sharing the same light cone
    in a 2D circuit with arbitrary connectivity.

    Parameters:
    - N: Total number of qubits
    - depth: Circuit depth
    - gates_info: List of gates with associated qubit indices and matrices

    Returns:
    - A list of reduced light cone groups with updated 'local_projection_matrix' for each group
    """
    def get_subset_indices(subset, big_set):
        return [big_set.index(item) for item in subset]
    # Step 1: Compute the light cone for each qubit
    print("Step 3: Compute the light cone for each qubit")
    light_cones = light_cone_arbitrary_connectivity_circuit(Identity_circuit_info, n_qubits_1, depth_1+depth_2)

    # print("light cones:", light_cones)
    max_light_cone_size = 0
    for q in range(n_qubits_1):
        light_cone_size = np.sum(light_cones[q, -1, :])
        if light_cone_size > max_light_cone_size:
            max_light_cone_size = light_cone_size
    print("Max light cone size:", max_light_cone_size)

    # Step 2: Group qubits that share the same light cone structure
    print("Step 4: Group qubits that share the same light cone structure")
    reduce_light_cones = reduce_light_cone_dictionary(light_cones)

    # Step 3: Organize the gates into circuit layers
    print("Step 5: Organize the gates into circuit layers")
    layers, depth = Manipulate_layers.divide_circuit_into_layers_using_layer_index(Identity_circuit_info)

    # print("Layers divided:", layers)

    # Step 4: Process each group of qubits with identical light cone
    print("Step 6: Process each group of qubits with identical light cone")
    for group_index, light_cone_group in enumerate(reduce_light_cones):
        print("----------------", flush=True)
        print("Light cone group:", group_index, flush=True)
        print("Qubits in light cone group:", light_cone_group['qubits'], flush=True)

        # Step 5: Iterate through each layer in depth
        for i in range(depth):
            
            cone = light_cone_group['light_cone']
            
            if i == 0:
                # First layer: initialize projection matrix to |00...0⟩⟨00...0|
                
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                s = list(['0' for _ in range(n_qubits_1)])
                for q in qubits_in_current_layer:
                    s[q] = '1'
                s = ''.join(s)
                previous_local_projection_matrix = qc.create_reduced_density_matrix(s)
                indices = ''.join(['0' for _ in range(n_qubits_1)])
                gate_full_matrix = qc.create_i(indices)

                # Apply the first valid gate in the layer (for efficiency)
                for gates in layers[i]:

                    # Sanity check: gate must be fully within the current light cone
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        print("Group index:", group_index)
                        print("Layer index:", i)
                        print("gate qubits:", gates['qubits'])
                        print("current light cone qubits:", qubits_in_current_layer)
                        print("full light cone:", cone[i])
                        print("full gate info:", gates)
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        gate_index_in_current_layer = get_subset_indices(gates['qubits'], qubits_in_current_layer)
                        # gate_full_matrix = embed_unitary_matrix(
                        #     operator_matrix=np.array(gates['matrix']),
                        #     target_qubits=gate_index_in_current_layer,
                        #     n_qubits=len(qubits_in_current_layer))
                        gate_full_matrix = gates['matrix']
                        break  # Only one gate applied for now

                # Update the projection matrix
                if len(qubits_in_current_layer) == 0:
                    # print("Assigning previous projection matrix directly (no qubits in light cone), group", group_index, "at layer", i, flush=True)
                    current_local_projection_matrix = previous_local_projection_matrix
                else:
                    # print("Computing Reduced Density Matrix for group", group_index, "at layer", i, flush=True)
                    current_local_projection_matrix = qc.compute_reduced_density_matrix(gate_full_matrix, previous_local_projection_matrix)
                    # current_local_projection_matrix = gate_full_matrix @ previous_local_projection_matrix @ np.conjugate(np.transpose(gate_full_matrix))

            else:
                
                # For subsequent layers, update light cone and embed previous projection
                qubit_in_previous_layer = qubits_in_current_layer.copy()
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))

                previous_local_projection_matrix = current_local_projection_matrix
                indices = ''.join(['0' for _ in range(n_qubits_1)])
                current_unitary = qc.create_i(indices)
                # print ("Layer", i, "qubit:", light_cone_group["qubits"],"->",qubits_in_current_layer, flush=True)

                count = 0  # Count valid gates in the current light cone
                for gates in layers[i]:
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        print("Group index:", group_index)
                        print("Layer index:", i)
                        print("gate qubits:", gates['qubits'])
                        print("current light cone qubits:", qubits_in_current_layer)
                        print("full light cone:", cone[i])
                        print("full gate info:", gates)
                        return False


                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        count += 1

                        # print("Gate in layer", i, ":", gates['qubits'], ":", gates['type'], flush=True)
                        if count == 1:
                            current_unitary = gates['matrix']
                            if apply_sequential:
                                current_local_projection_matrix = qc.compute_reduced_density_matrix(current_unitary, current_local_projection_matrix)
                                current_unitary = qc.create_i(indices)
                        else:
                            gate_full_matrix = gates['matrix']

                            # current_unitary = gate_full_matrix @ current_unitary
                            if apply_sequential:
                                current_local_projection_matrix = qc.compute_reduced_density_matrix(gate_full_matrix, current_local_projection_matrix)
                            else:
                                current_unitary = qc.gate_gate_apply(gate_full_matrix, current_unitary)

                        # print("current unitary during computing: ", flush=True) 
                        # current_unitary.print()

                # Update the local projection matrix
                if len(qubits_in_current_layer) == 0:
                    # print("Assigning previous projection matrix directly (no qubits in light cone), group", group_index, "at layer", i, flush=True)
                    current_local_projection_matrix = previous_local_projection_matrix
                else:
                    # print("Computing Reduced Density Matrix for group", group_index, "at layer", i, flush=True)
                    # current_local_projection_matrix = current_unitary @ previous_local_projection_matrix @ np.conjugate(np.transpose(current_unitary))
                    if not apply_sequential:
                        # print("Printing current unitary:", flush=True)
                        # current_unitary.print()
                        # print("Printing previous local projection matrix:", flush=True)
                        # previous_local_projection_matrix.print()
                        current_local_projection_matrix = qc.compute_reduced_density_matrix(current_unitary, previous_local_projection_matrix)
            
            if group_index == 1:
                print("After layer", i, "computing size", flush=True)
                current_local_projection_matrix.print()
        # Assign the resulting matrix back to the group
        light_cone_group['local_projection'] = current_local_projection_matrix
        
        # target_eigenvector = np.zeros(current_local_projection_matrix.shape[0], dtype=np.complex128)
        # target_eigenvector[0] = 1
        target_eigenvector = qc.get_init_density()

        # Apply projection
        # projection_result = current_local_projection_matrix @ target_eigenvector
        print ("Before computing L1 norm, group", group_index, flush=True)
        # current_local_projection_matrix.print()
        projection_result = qc.density_density_apply(current_local_projection_matrix, target_eigenvector)

        # l1_norm = np.linalg.norm(projection_result - target_eigenvector, ord=1)
        l1_norm = qc.compute_l1_norm(projection_result, target_eigenvector)
        # Check if projection result matches the original vector
        tolerance_level = (light_cone_group['light_cone'][-1] == 1).sum() * tolerance
        # tolerance_level = local_projection_computation.shape[0] * tolerance
        print ("Group", group_index, "L1 norm:", l1_norm, "Tolerance level:", tolerance_level, flush=True)
        if l1_norm > tolerance_level:
            print("Projection check failed at group", group_index)
            print("L1 norm:", l1_norm, "Tolerance:", tolerance_level)
            return False, max_light_cone_size
        if group_index % 10 == 0:
            print("Projection check passed at group", group_index)
        # print("Projection result size:", projection_result.size())
    # All local projections passed
    return True, max_light_cone_size

def local_projection_check_if_two_circuits_are_equal_using_numpy(qc_info_1, qc_info_2, tolerance=1e-15, qc=None):
    """
    Check if two quantum circuits are equivalent using local projection matrices.
    
    This function works by:
    1. Computing the inverse of the second circuit.
    2. Combining the first circuit with the inverse of the second to form a "difference circuit."
    3. Computing local projections of the difference circuit.
    4. Verifying that the resulting projections all preserve the |0...0⟩ state.

    Parameters:
    - qc_info_1: List of gate information from circuit 1
    - qc_info_2: List of gate information from circuit 2
    - tolerance: Tolerance for comparing eigenvector preservation (default = 1e-15)

    Returns:
    - True if circuits are locally equivalent
    - False otherwise
    """

    # Step 1: Get circuit size and depth
    print("Step 1: Get circuit size and depth")
    n_qubits_1, depth_1 = Manipulate_layers.find_circuit_properties(qc_info_1)
    n_qubits_2, depth_2 = Manipulate_layers.find_circuit_properties(qc_info_2)

    # Optional check for same circuit structure (commented out in case inverse makes them match)
    # if n_qubits_1 != n_qubits_2 or depth_1 != depth_2:
    #     print('Two circuits have different qubit counts or depths')
    #     return False

    # Step 2: Invert the second circuit and concatenate it with the first
    print("Step 2: Invert the second circuit and concatenate it with the first")
    _, inverse_qc_info_2 = Create_quantum_circuit.load_inverse_circuit_from_gate_info(
        qc_info_2, n_qubits=n_qubits_1)
    Identity_circuit_info = Create_quantum_circuit.combine_circuits_info(
        qc_info_1, inverse_qc_info_2)



    """
    Compute local projection matrices for groups of qubits sharing the same light cone
    in a 2D circuit with arbitrary connectivity.

    Parameters:
    - N: Total number of qubits
    - depth: Circuit depth
    - gates_info: List of gates with associated qubit indices and matrices

    Returns:
    - A list of reduced light cone groups with updated 'local_projection_matrix' for each group
    """
    def get_subset_indices(subset, big_set):
        return [big_set.index(item) for item in subset]
    # Step 1: Compute the light cone for each qubit
    print("Step 3: Compute the light cone for each qubit")
    light_cones = light_cone_arbitrary_connectivity_circuit(Identity_circuit_info, n_qubits_1, depth_1+depth_2)

    # print("light cones:", light_cones)
    max_light_cone_size = 0
    for q in range(n_qubits_1):
        light_cone_size = np.sum(light_cones[q, -1, :])
        if light_cone_size > max_light_cone_size:
            max_light_cone_size = light_cone_size
    print("Max light cone size:", max_light_cone_size)

    # Step 2: Group qubits that share the same light cone structure
    print("Step 4: Group qubits that share the same light cone structure")
    reduce_light_cones = reduce_light_cone_dictionary(light_cones)

    # Step 3: Organize the gates into circuit layers
    print("Step 5: Organize the gates into circuit layers")
    layers, depth = Manipulate_layers.divide_circuit_into_layers_using_layer_index(Identity_circuit_info)

    # print("Layers divided:", layers)

    # Step 4: Process each group of qubits with identical light cone
    print("Step 6: Process each group of qubits with identical light cone")
    for group_index, light_cone_group in enumerate(reduce_light_cones):
        # print("----------------")
        # print("Light cone group:", group_index)
        # print("Qubits in light cone group:", light_cone_group['qubits'])

        # Step 5: Iterate through each layer in depth
        for i in range(depth):
            
            cone = light_cone_group['light_cone']
            
            if i == 0:
                # First layer: initialize projection matrix to |00...0⟩⟨00...0|
                
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                previous_local_projection_matrix = np.zeros(
                    (2**len(qubits_in_current_layer), 2**len(qubits_in_current_layer)), dtype=complex)
                previous_local_projection_matrix[0, 0] = 1  # |00⟩⟨00|
                gate_full_matrix = np.eye(2)

                # Apply the first valid gate in the layer (for efficiency)
                for gates in layers[i]:

                    # Sanity check: gate must be fully within the current light cone
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        print("Group index:", group_index)
                        print("Layer index:", i)
                        print("gate qubits:", gates['qubits'])
                        print("current light cone qubits:", qubits_in_current_layer)
                        print("full light cone:", cone[i])
                        print("full gate info:", gates)
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        gate_index_in_current_layer = get_subset_indices(gates['qubits'], qubits_in_current_layer)
                        gate_full_matrix = embed_unitary_matrix(
                            operator_matrix=np.array(gates['matrix']),
                            target_qubits=gate_index_in_current_layer,
                            n_qubits=len(qubits_in_current_layer))
                        break  # Only one gate applied for now

                # Update the projection matrix
                if len(qubits_in_current_layer) == 0:
                    current_local_projection_matrix = previous_local_projection_matrix
                else:
                    current_local_projection_matrix = gate_full_matrix @ previous_local_projection_matrix @ np.conjugate(gate_full_matrix)
            else:
                
                # For subsequent layers, update light cone and embed previous projection
                qubit_in_previous_layer = qubits_in_current_layer.copy()
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                index_in_current_layer = get_subset_indices(qubit_in_previous_layer, qubits_in_current_layer)

                # Embed previous projection matrix into new (larger) space
                previous_local_projection_matrix = embed_unitary_matrix(
                    current_local_projection_matrix, index_in_current_layer, len(qubits_in_current_layer))
                
                current_unitary = np.eye(2**len(qubits_in_current_layer))
                current_embedded_qubits = qubits_in_current_layer

                count = 0  # Count valid gates in the current light cone
                for gates in layers[i]:
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        print("Group index:", group_index)
                        print("Layer index:", i)
                        print("gate qubits:", gates['qubits'])
                        print("current light cone qubits:", qubits_in_current_layer)
                        print("full light cone:", cone[i])
                        print("full gate info:", gates)
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        count += 1

                        if count == 1:
                            # Initialize unitary with the first gate
                            current_embedded_qubits = sorted(set(gates['qubits']))
                            current_unitary = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))
                        else:
                            # Merge and embed multiple gates into one operator
                            previous_embedded_qubits = current_embedded_qubits.copy()
                            current_embedded_qubits.extend(gates['qubits'])
                            current_embedded_qubits = sorted(set(current_embedded_qubits))

                            current_unitary = embed_unitary_matrix(
                                operator_matrix=current_unitary,
                                target_qubits=get_subset_indices(previous_embedded_qubits, current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            gate_full_matrix = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            current_unitary = gate_full_matrix @ current_unitary


                # Embed the composed unitary into the full light cone
                current_unitary = embed_unitary_matrix(
                    operator_matrix=current_unitary,
                    target_qubits=get_subset_indices(current_embedded_qubits, qubits_in_current_layer),
                    n_qubits=len(qubits_in_current_layer))

                # Update the local projection matrix
                if len(qubits_in_current_layer) == 0:
                    current_local_projection_matrix = previous_local_projection_matrix
                else:
                    current_local_projection_matrix = current_unitary @ previous_local_projection_matrix @ np.conjugate(current_unitary)    

        # Assign the resulting matrix back to the group
        light_cone_group['local_projection'] = current_local_projection_matrix
        
        target_eigenvector = np.zeros(current_local_projection_matrix.shape[0], dtype=np.complex128)
        target_eigenvector[0] = 1

        # Apply projection
        projection_result = current_local_projection_matrix @ target_eigenvector

        print("projection_result:", projection_result)
        print("target_eigenvector:", target_eigenvector)

        l1_norm = np.linalg.norm(projection_result - target_eigenvector, ord=1)

        # Check if projection result matches the original vector
        tolerance_level = projection_result.shape[0] * tolerance
        if l1_norm > tolerance_level:
            print("Projection check failed at group", group_index)
            print("L1 norm:", l1_norm, "Tolerance:", tolerance_level)
            return False, max_light_cone_size
        
        if group_index % 10 == 0:
            print("Projection check passed at group", group_index)
        # print("Projection result size:", projection_result.size())
    # All local projections passed
    return True, max_light_cone_size


def Choi_isomorphism_local_projection_check_if_two_circuits_are_equal(qc_info_1, qc_info_2, tolerance=1e-15):
    """
    Check if two quantum circuits are equivalent using local projection matrices.
    
    This function works by:
    1. Computing the inverse of the second circuit.
    2. Combining the first circuit with the inverse of the second to form a "difference circuit."
    3. Computing local projections of the difference circuit.
    4. Verifying that the resulting projections all preserve the |0...0⟩ state.

    Parameters:
    - qc_info_1: List of gate information from circuit 1
    - qc_info_2: List of gate information from circuit 2
    - tolerance: Tolerance for comparing eigenvector preservation (default = 1e-15)

    Returns:
    - True if circuits are locally equivalent
    - False otherwise
    """

    # Step 1: Get circuit size and depth
    n_qubits_1, depth_1 = Manipulate_layers.find_circuit_properties(qc_info_1)
    n_qubits_2, depth_2 = Manipulate_layers.find_circuit_properties(qc_info_2)

    # Optional check for same circuit structure (commented out in case inverse makes them match)
    # if n_qubits_1 != n_qubits_2 or depth_1 != depth_2:
    #     print('Two circuits have different qubit counts or depths')
    #     return False

    # Step 2: Invert the second circuit and concatenate it with the first
    _, inverse_qc_info_2 = Create_quantum_circuit.load_inverse_circuit_from_gate_info(
        qc_info_2, n_qubits=n_qubits_1)
    Identity_circuit_info = Create_quantum_circuit.combine_circuits_info(
        qc_info_1, inverse_qc_info_2)


    """
    Compute local projection matrices for groups of qubits sharing the same light cone
    in a 2D circuit with arbitrary connectivity.

    Parameters:
    - N: Total number of qubits
    - depth: Circuit depth
    - gates_info: List of gates with associated qubit indices and matrices

    Returns:
    - A list of reduced light cone groups with updated 'local_projection_matrix' for each group
    """
    def get_subset_indices(subset, big_set):
        return [big_set.index(item) for item in subset]
    # Step 1: Compute the light cone for each qubit
    light_cones = light_cone_arbitrary_connectivity_circuit(Identity_circuit_info, n_qubits_1, depth_1+depth_2)

    # Step 2: Group qubits that share the same light cone structure
    reduce_light_cones = reduce_light_cone_dictionary(light_cones)

    # Step 3: Organize the gates into circuit layers
    layers, depth = Manipulate_layers.divide_circuit_into_layers_using_layer_index(Identity_circuit_info)
    EPR_projector_AB = 1/2 * np.array([[1, 0, 0, 1], [0, 0, 0, 0],[0, 0, 0, 0],[1, 0, 0, 1]], dtype=np.complex128)
    def split_matrix_into_named_blocks(A):
        """
        Splits a square matrix into 4 blocks and returns them with specific names (B_00, B_01, B_10, B_11).
        
        Arguments:
        - A (ndarray): A square matrix of size 2^n x 2^n.

        Returns:
        - blocks (tuple): A tuple containing the four blocks:
        (B_00, B_01, B_10, B_11).
        """
        # Get the size of the matrix
        size = A.shape[0]
        
        # Ensure the matrix size is a power of 2
        if size % 2 != 0:
            raise ValueError("Matrix size must be a power of 2.")
        
        # Compute the middle index (for splitting the matrix)
        mid = size // 2
        
        # Split the matrix into four blocks
        B_00 = A[:mid, :mid]  # Top-left block
        B_01 = A[:mid, mid:]  # Top-right block
        B_10 = A[mid:, :mid]  # Bottom-left block
        B_11 = A[mid:, mid:]  # Bottom-right block
        
        return B_00, B_01, B_10, B_11
    def reconstruct_matrix_M_ACB(M_AB, M_C):
        Blocks_B = split_matrix_into_named_blocks(M_AB)
        A_00 = np.array([[1,0],[0,0]])
        A_01 = np.array([[0,1],[0,0]])
        A_10 = np.array([[0,0],[1,0]])
        A_11 = np.array([[0,0],[0,1]])
        M_ABC_00 = np.kron(A_00,np.kron(M_C,Blocks_B[0]))
        # print(M_ABC_00)
        M_ABC_01 = np.kron(A_01,np.kron(M_C,Blocks_B[1]))
        M_ABC_10 = np.kron(A_10,np.kron(M_C,Blocks_B[2]))
        M_ABC_11 = np.kron(A_11,np.kron(M_C,Blocks_B[3]))
        return M_ABC_00+M_ABC_01+M_ABC_10+M_ABC_11
    def qubit_set_mapping(qubits_in_current_layer):
        return sorted([qubit + 1 for qubit in qubits_in_current_layer] + [0])
    def qubit_set_mapping_without_0(qubits_in_current_layer):
        return ([qubit + 1 for qubit in qubits_in_current_layer])
    # print("Layers divided:", layers)

    # Step 4: Process each group of qubits with identical light cone
    for group_index, light_cone_group in enumerate(reduce_light_cones):
        # print("----------------")
        # print("Light cone group:", group_index)
        # print("Qubits in light cone group:", light_cone_group['qubits'])

        # Step 5: Iterate through each layer in depth
        for i in range(depth):
            # print("Layer", i)
            cone = light_cone_group['light_cone']
            # print("check_point_1")
            if i == 0:
                # First layer: initialize projection matrix to |00...0⟩⟨00...0|
                # print(cone[i])
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                # print(qubits_in_current_layer)
                qubits_in_current_layer_mapped = sorted([qubit + 1 for qubit in qubits_in_current_layer] + [0])
                # print(qubits_in_current_layer)
                initial_qubit = qubits_in_current_layer[0]
                previous_local_projection_matrix = embed_unitary_matrix(EPR_projector_AB,[0,1],len(qubits_in_current_layer)+1)
                # print(len(qubits_in_current_layer))
                # print(previous_local_projection_matrix)
                # np.zeros(
                #     (2**len(qubits_in_current_layer), 2**len(qubits_in_current_layer)), dtype=complex)
                # previous_local_projection_matrix[0, 0] = 1  # |00⟩⟨00|

                # print("Qubits in current layer:", qubits_in_current_layer)

                # Apply the first valid gate in the layer (for efficiency)
                for gates in layers[i]:
                    # print("Gate in layer", i, ":", gates['qubits'])
                    # print("check_point_2")

                    # Sanity check: gate must be fully within the current light cone
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        gate_index_in_current_layer = get_subset_indices(gates['qubits'], qubits_in_current_layer)
                        # print("check_point_3")
                        gate_full_matrix = embed_unitary_matrix(
                            operator_matrix=np.array(gates['matrix']),
                            target_qubits=qubit_set_mapping_without_0(gate_index_in_current_layer),
                            n_qubits=len(qubits_in_current_layer)+1)
                        # print("current unitary: ", gate_full_matrix)
                        # print("dsdsdd")
                        # print("original gate: ", gates['matrix'])
                        break  # Only one gate applied for now

                # Update the projection matrix
                current_local_projection_matrix = gate_full_matrix @ previous_local_projection_matrix @ np.conjugate(np.transpose(gate_full_matrix))
                # print("done layer 0")
                # print(current_local_projection_matrix.shape)

            else:
                # print(cone[i])
                # For subsequent layers, update light cone and embed previous projection
                qubit_in_previous_layer = qubits_in_current_layer.copy()
                qubits_in_current_layer = sorted(set(np.where(cone[i] == 1)[0]))
                index_in_current_layer = get_subset_indices(qubit_in_previous_layer, qubits_in_current_layer)
                # print(index_in_current_layer)
                # print("check_point_2")
                # Embed previous projection matrix into new (larger) space
                previous_local_projection_matrix = embed_unitary_matrix(
                    current_local_projection_matrix, qubit_set_mapping(index_in_current_layer), len(qubits_in_current_layer)+1)
                # print(qubit_set_mapping(index_in_current_layer))
                # print(previous_local_projection_matrix.shape)

                # print("Qubits in previous layer:", qubit_in_previous_layer)
                # print("Qubits in current layer:", qubits_in_current_layer)
                # print("Index in current layer:", index_in_current_layer)

                count = 0  # Count valid gates in the current light cone
                for gates in layers[i]:
                    if set(gates['qubits']).intersection(qubits_in_current_layer) and not set(gates['qubits']).issubset(qubits_in_current_layer):
                        print("Error: gate intersects but is not fully inside the cone")
                        return False

                    if set(gates['qubits']).issubset(qubits_in_current_layer):
                        count += 1
                        # print("Gate in layer", i, ":", gates['qubits'])

                        if count == 1:
                            # Initialize unitary with the first gate
                            current_embedded_qubits = sorted(set(gates['qubits']))
                            current_unitary = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))
                        else:
                            # Merge and embed multiple gates into one operator
                            previous_embedded_qubits = current_embedded_qubits.copy()
                            current_embedded_qubits.extend(gates['qubits'])
                            current_embedded_qubits = sorted(set(current_embedded_qubits))

                            current_unitary = embed_unitary_matrix(
                                operator_matrix=current_unitary,
                                target_qubits=get_subset_indices(previous_embedded_qubits, current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            gate_full_matrix = embed_unitary_matrix(
                                operator_matrix=np.array(gates['matrix']),
                                target_qubits=get_subset_indices(gates['qubits'], current_embedded_qubits),
                                n_qubits=len(current_embedded_qubits))

                            current_unitary = gate_full_matrix @ current_unitary
                        # print("current unitary: ", current_unitary)
                        # print("current_embbeded:", current_embedded_qubits)

                # print("count:", count)

                # Embed the composed unitary into the full light cone
                current_unitary = embed_unitary_matrix(
                    operator_matrix=current_unitary,
                    target_qubits=qubit_set_mapping_without_0(get_subset_indices(current_embedded_qubits, qubits_in_current_layer)),
                    n_qubits=len(qubits_in_current_layer)+1)
                # print("current unitary: ", current_unitary)
                # print("Final embedding for this layer:", current_embedded_qubits, qubits_in_current_layer)

                # Update the local projection matrix
                # print("Previous:", previous_local_projection_matrix)
                # print("check_point_3")
                current_local_projection_matrix = current_unitary @ previous_local_projection_matrix @ np.conjugate(np.transpose(current_unitary))

        # Assign the resulting matrix back to the group
        light_cone_group['local_projection'] = current_local_projection_matrix
        if not isinstance(current_local_projection_matrix, np.ndarray) or current_local_projection_matrix.shape[0] != current_local_projection_matrix.shape[1]:
            print(f"Invalid projection matrix in group {group_index}")
            return False
        

        test_density_matrix = EPR_projector_AB
        test_density_matrix = embed_unitary_matrix(EPR_projector_AB,[0,get_subset_indices([initial_qubit],qubits_in_current_layer)[0]+1],len(qubits_in_current_layer)+1)
        # Compute the image after applying the local projection
        image = current_local_projection_matrix @ test_density_matrix
        
        # Compare the resulting matrix with the original test_density_matrix

        if np.linalg.norm(image-test_density_matrix,ord=1)>image.shape[0]*tolerance:
            return False
        # if not np.allclose(image, test_density_matrix, atol=1e-14):
        #     # print("Inequivalence found")
        #     return False  # Return immediately if matrices are not equal

    # print("The circuits are equivalent.")
    return True
    #     target_eigenvector = np.zeros(current_local_projection_matrix.shape[0], dtype=np.complex128)
    #     target_eigenvector[0] = 1

    #     # Apply projection
    #     projection_result = current_local_projection_matrix @ target_eigenvector

    #     # Check if projection result matches the original vector
    #     if np.linalg.norm(projection_result - target_eigenvector, ord=1) > current_local_projection_matrix.shape[0] * tolerance:
    #         print("Projection check failed at group", group_index)
    #         return False

    # # All local projections passed
    # return True

    # # # Step 3: Compute local projections of the combined "difference circuit"
    # # # local_projections = local_projection_computation.compute_local_projections_fullly_general(gates_info=Identity_circuit_info,n_qubits=n_qubits_1,depth=depth_1+depth_2)
    # # local_projections = compute_local_projection_2d_circuit(
    # #     n_qubits_1,
    # #     depth=depth_1 + depth_2,
    # #     gates_info=Identity_circuit_info
    # # )

    # # Step 4: For each projection matrix, verify it preserves |0...0⟩ state
    # for count, entry in enumerate(local_projections, start=1):
    #     local_proj = entry['local_projection']

    #     # Sanity check: should be square matrix
    #     if not isinstance(local_proj, np.ndarray) or local_proj.shape[0] != local_proj.shape[1]:
    #         print(f"Invalid projection matrix in group {count}")
    #         return False

    #     # Define target state |0...0⟩
    #     target_eigenvector = np.zeros(local_proj.shape[0], dtype=np.complex128)
    #     target_eigenvector[0] = 1

    #     # Apply projection
    #     projection_result = local_proj @ target_eigenvector

    #     # Check if projection result matches the original vector
    #     if np.linalg.norm(projection_result - target_eigenvector, ord=1) > local_proj.shape[0] * tolerance:
    #         print("Projection check failed at group", count)
    #         return False

    # # All local projections passed
    # return True





def benchmark_local_projection(output_file="runtime_log.txt", max_L=32, depth=2):
    """
    Benchmarks local_projection_check_if_two_circuits_are_equal for increasing L values.

    Parameters:
    - output_file: str, the file to log results.
    - max_L: int, the maximum L value to test (inclusive).
    - depth: int, the depth of the quantum circuit.
    """
    with open(output_file, 'w') as f:
        f.write("L\truntime_seconds\n")  # Header
        for L in range(2, max_L + 1, 2):
            print(f"Running for L = {L}...")
            _, qc_info = create_2d_circuit(L, depth)
            
            start_time = time.time()
            result = compute_local_projection_2d_circuit(L*L,depth,qc_info)
            end_time = time.time()
            
            runtime = end_time - start_time
            print(f"L={L}, Time={runtime:.4f}s") #, Result={result}")
            f.write(f"{L}\t{runtime:.6f}\n")

# Example usage:
# benchmark_local_projection(max_L=16, depth=3)





