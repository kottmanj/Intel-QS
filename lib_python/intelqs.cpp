#include <pybind11/pybind11.h>
#include <pybind11/iostream.h>
#include <pybind11/stl.h>
#include <pybind11/stl_bind.h>
#include <pybind11/numpy.h>

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "qureg.hpp"

//////////////////////////////////////////////////////////////////////////////

namespace py = pybind11;

//////////////////////////////////////////////////////////////////////////////

//template class QubitRegister<ComplexDP>;

void TestSimulationWithIntelQS()
{
  unsigned num_logical_qubits = 2;
#if 10
  QubitRegister<ComplexDP> psi(num_logical_qubits, "base", 0);

  // probability of being in |1>, state left unchanged
  std::cout << "before Hadamard = " << psi.GetProbability(0) << "\n";
  psi.ApplyHadamard(0);
  std::cout << "after Hadamard = " << psi.GetProbability(0) << "\n";
#endif
}

//////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////
// PYBIND CODE for the QubitRegister class
//////////////////////////////////////////////////////////////////////////////

PYBIND11_MODULE(intelqs, m)
{
    m.doc() = "pybind11 wrap for the Intel Quantum Simulator";

    m.def("TestSimulation", &TestSimulationWithIntelQS, "Trivial circuit in qHiPSTER");

//////////////////////////////////////////////////////////////////////////////
// UTILITIES : RANDOM-NUMBER-GENERATOR
//////////////////////////////////////////////////////////////////////////////

#if 0
    // The random number generator.
    py::class_<utility::mpi::RandomNumberGenerator>(m, "RandomNumberGenerator")
        .def(py::init<>())
        .def("SetSeedAndInitializeRndStreamPtrs", &utility::mpi::RandomNumberGenerator::SetSeedAndInitializeRndStreamPtrs)
        .def("InitializeRndStreamPtrs", &utility::mpi::RandomNumberGenerator::InitializeRndStreamPtrs)
        .def("SetSeedStreamPtrs", &utility::mpi::RandomNumberGenerator::SetSeedStreamPtrs)
        .def("__repr__", []() { return "<RandomNumberGenerator based on MKL.>"; } );
#endif

//////////////////////////////////////////////////////////////////////////////
// qHiPSTER
//////////////////////////////////////////////////////////////////////////////

    // Intel Quantum Simulator
    // Notice that to use std::cout in the C++ code, one needs to redirect the output streams.
    // https://pybind11.readthedocs.io/en/stable/advanced/pycpp/utilities.html
//    py::class_<QubitRegister<ComplexDP>, shared_ptr< QubitRegister<ComplexDP> >>(m, "QubitRegister")
    py::class_< QubitRegister<ComplexDP> >(m, "QubitRegister", py::buffer_protocol(), py::dynamic_attr())
        .def(py::init<> ())
        .def(py::init<std::size_t , std::string , std::size_t, std::size_t> ())
        // One-qubit gates:
        .def("ApplyRotationX", &QubitRegister<ComplexDP>::ApplyRotationX)
        .def("ApplyRotationY", &QubitRegister<ComplexDP>::ApplyRotationY)
        .def("ApplyRotationZ", &QubitRegister<ComplexDP>::ApplyRotationZ)
        .def("ApplyPauliX", &QubitRegister<ComplexDP>::ApplyPauliX)
        .def("ApplyPauliY", &QubitRegister<ComplexDP>::ApplyPauliY)
        .def("ApplyPauliZ", &QubitRegister<ComplexDP>::ApplyPauliZ)
        .def("ApplyPauliSqrtX", &QubitRegister<ComplexDP>::ApplyPauliSqrtX)
        .def("ApplyPauliSqrtY", &QubitRegister<ComplexDP>::ApplyPauliSqrtY)
        .def("ApplyPauliSqrtZ", &QubitRegister<ComplexDP>::ApplyPauliSqrtZ)
        .def("ApplyT", &QubitRegister<ComplexDP>::ApplyT)
        .def("ApplyHadamard", &QubitRegister<ComplexDP>::ApplyHadamard)
        // Two-qubit gates:
        .def("ApplySwap", &QubitRegister<ComplexDP>::ApplySwap)
        .def("ApplyCPauliX", &QubitRegister<ComplexDP>::ApplyCPauliX)
        .def("ApplyCPauliZ", &QubitRegister<ComplexDP>::ApplyCPauliZ)
        // Other quantum operations:
        .def("ApplyToffoli", &QubitRegister<ComplexDP>::ApplyToffoli)
        .def("Initialize",
               (void (QubitRegister<ComplexDP>::*)(std::string, std::size_t ))
                 &QubitRegister<ComplexDP>::Initialize)
        .def("GetProbability", &QubitRegister<ComplexDP>::GetProbability)
        .def("ExpectationValue", &QubitRegister<ComplexDP>::ExpectationValue)
        .def("compute_ExpectationValue", &QubitRegister<ComplexDP>::compute_ExpectationValue)
        // Utility functions:
        .def("Print",
             [](QubitRegister<ComplexDP> &a, std::string description) {
               py::scoped_ostream_redirect stream(
               std::cout,                               // std::ostream&
               py::module::import("sys").attr("stdout") // Python output
               );
               std::vector<size_t> qubits = {};
               a.Print(description, qubits);
             }, "Print the quantum state with an initial description.");

}

//////////////////////////////////////////////////////////////////////////////
