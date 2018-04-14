#include <cstdio>
#include <llvm/Pass.h>
#include <llvm/IR/Function.h>

class EasyPass : public FunctionPass {
public:
  satic char ID;
  EasyPass() : FunctionPass(ID){}
  ~EasyPass(){}
  virtual bool runOnFunction(Function &F);
  virtual void getAnalysisUsage(llvm::AnalysisUsage &AU) const;
};


bool EasyPass::runOnFunction(Function &F) {
  llvm::errs() << "Funtion name" << F.getName().str() << "\n";
  return false;
};

void EasyPass::getAnalysisUsage(llvm::AnalysisUsage &AU) const{
  // not change
  AU.setPreservesAll();
};

char EasyPass::ID = 0;
static RegisterPass<EasyPass> X("easypass", "easy pass:only print functon name", false, false);
