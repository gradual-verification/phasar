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

  if (auto *F = DB.getFunctionDefinition("test")) {
    for (auto& A : F->args()) { 
      A.dump(); 
      DynamicPointsToSetConstPtr<psr::LLVMPointsToInfo::PointsToSetTy> pts = P.getPointsToSet(&A);
      psr::LLVMPointsToInfo::PointsToSetTy set = *pts;
      
    }
    // print inter-procedural control-flow graph

    
  } else {
    llvm::errs() << "error: file does not contain a 'main' function!\n";
  }
  return 0;
}
