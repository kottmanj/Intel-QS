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
#include <fstream>
#include <unordered_map>
#include <functional>
#include <stdexcept>

#include "qureg/qureg.hpp"
#include "interface_api_qasm.h"

using namespace std;


// Global variables related to Psi-function .malloc/.free routines.
using Type = ComplexDP;
QubitRegister<Type> *psi1 = nullptr;
bool fPsiAllocated = false;



int main(int argc, char*argv[]) {
    openqu::mpi::Environment env(argc, argv);
    string line = "";
    string token = "";
    string rest = "";

    if (env.is_usefull_rank() == false) 
        return 0;

    int myid = env.rank();
    using Type = ComplexDP;

    while(true) {
        getline(cin,line);

        if(line.length() >= 1) {
            int token_end = line.find_first_of(' ');
	    int par_open = line.find_first_of('(');
            int par_close = line.find_first_of(')');
            unsigned long result = 1;

            token = line.substr(0,token_end);
            string param = "";
	    if(par_open>=0){
                 token = line.substr(0,par_open);
                 param = line.substr(par_open+1,par_close-par_open-1);
            //std::cout << "line=" << line << "\n";
	    //std::cout << "param=" << param << "\n";        
                 rest = line.substr(par_close+1,line.length());
                 // Pyquil failsafe:
                 if(param=="pi/2") param = "1.5707963267948966";
                 if(param=="-pi/2") param = "-1.5707963267948966";
                 
		 rest += " "+param;
                 
	    }else{
                 token = line.substr(0,token_end);
                 rest = line.substr(token_end+1,line.length());
	    }    
	    //std::cout << "token=" << token << "\n";        
	    //std::cout << "rest =" << rest << "\n";        
	    //std::cout << "param=" << param << "\n";        
	    //std::cout << "( at" << par_open << "\n";
	    //std::cout << ") at" << par_close << "\n";

	    if(!token.empty()) {
	/*
               function<long(string)> func = qufun_table[token];
               if(func) {
                  result = func(line.substr(token_end+1,line.length()));
               }
*/
	       result = ExecuteHandler(token,rest);

               if (result > 0) {
                   cerr << "Qasm Op failed - ["<<token<<"]"<<endl;
               }
            }
        } else
          break;
    }

return 0;
}
