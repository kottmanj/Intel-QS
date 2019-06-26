//------------------------------------------------------------------------------
// Copyright 2017 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//------------------------------------------------------------------------------
#include <iostream>
#include <unordered_map>
#include <functional>
#include <stdexcept>

#include "../qureg/qureg.hpp"
#include "interface_api_qubitid.h"
#include "interface_api_version.h"
#include "interface_api_memory.h"


using namespace std;


using Type = ComplexDP;
extern QubitRegister<Type> *psi1;


// Constant defining the rotational angle of a T-dagger gate. Basically, -(pi/4).
#define TDAG_THETA -0.785398163397448


unsigned long unk(string args) {
    return 1;
}

unsigned long print_handler(string args) {
	psi1->Print("current state");
	return 0;
}

std::string extract_from_par(const std::string& s){
	int start = s.find_first_of("[");
	int end = s.find_first_of("]");
	return s.substr(start+1,end-start-1);
}

unsigned long expectation_handler(string args) {
	std::stringstream ss(args);
        std::vector<unsigned int> qubits;
	std::vector<unsigned int> paulis;
	string word;
	while(ss>>word){
		//std::cout << "word=" << word << "\n";
		char pauli=word[0];
		int ipauli=0;
		if(pauli=='X') ipauli=1;
		else if(pauli=='Y') ipauli=2;
     		else if(pauli=='Z') ipauli=3;
		paulis.push_back(ipauli);
		
		std::string qubit=extract_from_par(word);
		std::cout << word << " -> " << qubit << "\n";
		int iqubit=query_qubit_id(qubit);
		qubits.push_back(iqubit);
	}	
	
	auto result=psi1->compute_ExpectationValue(qubits, paulis);
	cout << "expectation value: " << args << " value=" <<  result << "\n";
	return 0;
}

unsigned long RX_handler(string args) {
    cout << "RX"<< " [" << args << "]" <<endl;
    std::stringstream ss(args);
    std::string args_qubit;
    double angle;
    ss >> args_qubit;
    ss >> angle;
    psi1->ApplyRotationX(query_qubit_id(args_qubit), angle);
    return 0;
}

unsigned long RY_handler(string args) {
    cout << "RY"<< " [" << args << "]" <<endl;
    std::stringstream ss(args);
    std::string args_qubit;
    double angle;
    ss >> args_qubit;
    ss >> angle;
    psi1->ApplyRotationY(query_qubit_id(args_qubit), angle);
    return 0;
}

unsigned long RZ_handler(string args) {
    cout << "RZ"<< " [" << args << "]" <<endl;
    std::stringstream ss(args);
    std::string args_qubit;
    double angle;
    ss >> args_qubit;
    ss >> angle;
    psi1->ApplyRotationZ(query_qubit_id(args_qubit), angle);
    return 0;
}





unsigned long S_handler(string args) {
    cout << "S"<< " [" << args << "]" <<endl;
    psi1->ApplyPauliSqrtZ(query_qubit_id(args));
    return 0;
}


unsigned long X_handler(string args) {
    cout << "X"<< " [" << args << "]" <<endl;
    psi1->ApplyPauliX(query_qubit_id(args));
    return 0;
}


unsigned long T_handler(string args) {
    cout << "T"<< " [" << args << "]" <<endl;
    psi1->ApplyT(query_qubit_id(args));
    return 0;
}


unsigned long Tdag_handler(string args) {
    cout << "Tdag"<< " [" << args << "]" <<endl;
    psi1->ApplyRotationZ(query_qubit_id(args),TDAG_THETA);
    return 0;
}


unsigned long CNOT_handler(string args) {
    int qubit1,
        qubit2;
    int token_end = args.find_first_of(',');
    if(token_end<0) token_end = args.find_first_of(' ');

    qubit1 = query_qubit_id(args.substr(0,token_end));
    qubit2 = query_qubit_id(args.substr(token_end+1,args.length()));

    cout << "CNOT"<< " [" << args << "]" <<endl;
    psi1->ApplyCPauliX(qubit1,qubit2);
    return 0;
}


unsigned long H_handler(string args) {
    cout << "H"<< " [" << args << "]" <<endl;
    psi1->ApplyHadamard(query_qubit_id(args));
    return 0;
}


unsigned long MeasZ_handler(string args) {
    using Type = ComplexDP;
    Type measurement = 0.0;
    
    cout << "MeasZ"<< " [" << args << "]" <<endl;
    measurement = psi1->GetProbability(query_qubit_id(args));
    cout << measurement << endl;
    return 0;
}


unsigned long PrepZ_handler(string args) {
    cout << "PrepZ"<< " [" << args << "]" <<endl;
    return 0;
}


// Hash table containing the QASM operation string and the function to call to
// handle the operation with the qHiPSTER simulation.
//
unordered_map<string, function<long(string)>> qufun_table = {\
                                                {".malloc", qumalloc},
                                                {".free", qufree},
                                                {".iversion",quiversion},
                                                {".version",quversion},
                                                {"RX", RX_handler},
                                                {"RY", RY_handler},
                                                {"RZ", RZ_handler},
						{"H", H_handler},
                                                {"CNOT", CNOT_handler},
                                                {"PrepZ",PrepZ_handler},
                                                {"T", T_handler},
                                                {"X", X_handler},
                                                {"Tdag", Tdag_handler},
                                                {"S", S_handler},
                                                {"MeasZ", MeasZ_handler},
                                                {"print", print_handler},
                                                {"expectation", expectation_handler},
                                                {"*", unk},
};



unsigned long ExecuteHandler(string op, string args) {

    unsigned long result = 1;

    function<long(string)> func = qufun_table[op];

    if(func) {
        result = func(args);
    }

    return result;
}
