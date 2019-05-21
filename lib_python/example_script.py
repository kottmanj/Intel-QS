import numpy
from numpy import random_intel # crucial to import this before the intelqs
import intelqs as simulator
import subprocess
from mpi4py import MPI # crucial to import this

#------------------------------------------------
#- Quantum Simulation ---------------------------
#------------------------------------------------


print("Creation of the QuantumRegister object.");
num_qubits = 4;
qreg = simulator.QubitRegister(num_qubits, "base", 0, 0);

print("Initialize to 1= |1>|0>|0>|0>.");
qreg.Initialize("base",1);

print("Get probabilities :\n q(0) has prob. {} to be in |1>\n q(1) has prob. {} to be in |1>".format(qreg.GetProbability(0),qreg.GetProbability(1)));

print("Compute PauliWords Expectation Value: XZII")
qubits = [0,1] # the qubits to measure
paulis = [3,3] # the pauli words: x=1, y=2, z=3)
expv=qreg.compute_ExpectationValue(qubits,paulis)
print("expectationvalue = ", expv)

print("Apply X(3) to obtain state |1>|0>|0>|1>.");
qreg.ApplyPauliX(3);

print("Compute PauliWords Expectation Value: IZIZ")
qubits = [1,3] # the qubits to measure
paulis = [3,3] # the pauli words: x=1, y=2, z=3)
expv=qreg.compute_ExpectationValue(qubits,paulis)
print("expectationvalue = ", expv)

print("Print state:");
qreg.Print("State should be |1001>");

print("now apply a Hadamard gate to get to |1+01>")
qreg.ApplyHadamard(1)

print("Compute PauliWords Expectation Value: IXII")
qubits = [1] # the qubits to measure
paulis = [1] # the pauli words: x=1, y=2, z=3)
expv=qreg.compute_ExpectationValue(qubits,paulis)
print("expectationvalue = ", expv)

print("some more expectation values:")
print("X(1)",qreg.compute_ExpectationValue([1],[1]))
print("Z(1)",qreg.compute_ExpectationValue([1],[1]))
print("Z(0)X(1)",qreg.compute_ExpectationValue([0,1],[3,1]))


print("Print state:");
qreg.Print("State should be |1+01> = |1001> + |1101>");

print("Apply a CNOT(1,0)")
control=1
target=0
qreg.ApplyCPauliX(control,target)

print("Print state:");
qreg.Print("State should be |1+01> = |1001> + |0101>");

#------------------------------------------------
