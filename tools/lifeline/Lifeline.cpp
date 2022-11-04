/******************************************************************************
 * Copyright (c) 2017 Philipp Schubert.
 * All rights reserved. This program and the accompanying materials are made
 * available under the terms of LICENSE.txt.
 *
 * Contributors:
 *     Philipp Schubert and others
 *****************************************************************************/

#include <filesystem>
#include <fstream>
#include <iostream>
#include<string>

#include "phasar/DB/ProjectIRDB.h"
#include "phasar/PhasarLLVM/ControlFlow/LLVMBasedICFG.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/IDELinearConstantAnalysis.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Problems/IFDSLinearConstantAnalysis.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Solver/IDESolver.h"
#include "phasar/PhasarLLVM/DataFlowSolver/IfdsIde/Solver/IFDSSolver.h"
#include "phasar/PhasarLLVM/Pointer/DynamicPointsToSetPtr.h"
#include "phasar/PhasarLLVM/Pointer/LLVMPointsToSet.h"
#include "phasar/PhasarLLVM/Pointer/LLVMBasedPointsToAnalysis.h"
#include "phasar/PhasarLLVM/Pointer/PointsToInfo.h"
#include "llvm/Analysis/AliasAnalysis.h"
#include "phasar/PhasarLLVM/TypeHierarchy/LLVMTypeHierarchy.h"
#include "phasar/Utils/Logger.h"

namespace llvm {
class Value;
} // namespace llvm


using namespace psr;


class LifetimeName{
    private:
      std::string name;       
      unsigned int indirection;
    public:
      LifetimeName(std::string arg_name, unsigned int arg_indirection){
        name = arg_name;
        indirection = arg_indirection;
      };
      std::string str(){
        std::string indirection_str = std::to_string(indirection);
        indirection_str.insert(0, LifetimeName::name);
        return indirection_str;
      }
};

class NameGenerator{
  private:
    std::string prefix;       
    char current;
  public:            
    NameGenerator() {     // Constructor
      prefix = "";
      current = 'a';
    };
    LifetimeName create () {
      LifetimeName N(prefix + current, 0);
      if(current == 'z'){
        current = 'a';
      }else{
        current = current + 1;
      }
      return N;
    };

};


int main(int Argc, const char **Argv) {
  if (Argc < 2 || !std::filesystem::exists(Argv[1]) ||
      std::filesystem::is_directory(Argv[1])) {
    llvm::errs() << "myphasartool\n"
                    "A small PhASAR-based example program\n\n"
                    "Usage: myphasartool <LLVM IR file>\n";
    return 1;
  }
  ProjectIRDB DB({Argv[1]});
  LLVMTypeHierarchy H(DB);
  LLVMPointsToSet P(DB, true, PointerAnalysisType::CFLSteens);
  //P.print();

  NameGenerator names;
  if (auto *F = DB.getFunctionDefinition("test")) {
    for (auto& A : F->args()) { 
      LifetimeName n = names.create();

      std::cout << "'" << n.str() << std::endl;
      std::string argname;
      llvm::raw_string_ostream aoutput(argname);
      A.print(aoutput);
      
      std::cout << "A: " << argname << std::endl;

      DynamicPointsToSetConstPtr<psr::LLVMPointsToInfo::PointsToSetTy> pts = P.getPointsToSet(&A);
      for (const llvm::Value *const val : *pts){
          std::string vname;
          llvm::raw_string_ostream voutput(vname);
          val->print(voutput);
          std::cout << "   -> " << vname << std::endl;           
      }
    }
  } else {
    llvm::errs() << "error: file does not contain a 'main' function!\n";
  }
  return 0;
}
