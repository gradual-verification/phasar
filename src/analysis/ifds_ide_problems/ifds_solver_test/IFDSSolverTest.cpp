/*
 * IFDSSolverTest.cpp
 *
 *  Created on: 31.05.2017
 *      Author: philipp
 */

#include "IFDSSolverTest.hh"

IFDSSolverTest::IFDSSolverTest(LLVMBasedICFG &I, vector<string> EntryPoints)
    : DefaultIFDSTabulationProblem<const llvm::Instruction *,
                                   const llvm::Value *, const llvm::Function *,
                                   LLVMBasedICFG &>(I),
      EntryPoints(EntryPoints) {
  DefaultIFDSTabulationProblem::zerovalue = createZeroValue();
}

shared_ptr<FlowFunction<const llvm::Value *>>
IFDSSolverTest::getNormalFlowFunction(const llvm::Instruction *curr,
                                      const llvm::Instruction *succ) {
  cout << "IFDSSolverTest::getNormalFlowFunction()\n";
  if (auto Store = llvm::dyn_cast<llvm::StoreInst>(curr)) {
    return make_shared<Gen<const llvm::Value *>>(
        Store->getPointerOperand(), DefaultIFDSTabulationProblem::zerovalue);
  }
  return Identity<const llvm::Value *>::v();
}

shared_ptr<FlowFunction<const llvm::Value *>>
IFDSSolverTest::getCallFlowFunction(const llvm::Instruction *callStmt,
                                   const llvm::Function *destMthd) {
  cout << "IFDSSolverTest::getCallFlowFunction()\n";
  return Identity<const llvm::Value *>::v();
}

shared_ptr<FlowFunction<const llvm::Value *>>
IFDSSolverTest::getRetFlowFunction(const llvm::Instruction *callSite,
                                   const llvm::Function *calleeMthd,
                                   const llvm::Instruction *exitStmt,
                                   const llvm::Instruction *retSite) {
  cout << "IFDSSolverTest::getRetFlowFunction()\n";
  return Identity<const llvm::Value *>::v();
}

shared_ptr<FlowFunction<const llvm::Value *>>
IFDSSolverTest::getCallToRetFlowFunction(const llvm::Instruction *callSite,
                                         const llvm::Instruction *retSite) {
  cout << "IFDSSolverTest::getCallToRetFlowFunction()\n";
  return Identity<const llvm::Value *>::v();
}

shared_ptr<FlowFunction<const llvm::Value *>>
IFDSSolverTest::getSummaryFlowFunction(const llvm::Instruction *callStmt,
                                       const llvm::Function *destMthd,
                                       vector<const llvm::Value *> inputs,
                                       vector<bool> context) {
  cout << "IFDSSolverTest::getSummaryFlowFunction()\n";
  return Identity<const llvm::Value *>::v();
}

map<const llvm::Instruction *, set<const llvm::Value *>>
IFDSSolverTest::initialSeeds() {
  cout << "IFDSSolverTest::initialSeeds()\n";
  map<const llvm::Instruction *, set<const llvm::Value *>> SeedMap;
  for (auto &EntryPoint : EntryPoints) {
    SeedMap.insert(std::make_pair(&icfg.getMethod(EntryPoint)->front().front(),
                                  set<const llvm::Value *>({zeroValue()})));
  }
  return SeedMap;
}

const llvm::Value *IFDSSolverTest::createZeroValue() {
  // create a special value to represent the zero value!
  static ZeroValue *zero = new ZeroValue;
  return zero;
}

bool IFDSSolverTest::isZeroValue(const llvm::Value* d) const {
  return isLLVMZeroValue(d);
}

string IFDSSolverTest::D_to_string(const llvm::Value *d) {
  return llvmIRToString(d);
}

string IFDSSolverTest::N_to_string(const llvm::Instruction *n) {
  return llvmIRToString(n);
}

string IFDSSolverTest::M_to_string(const llvm::Function *m) {
  return m->getName().str();
}
