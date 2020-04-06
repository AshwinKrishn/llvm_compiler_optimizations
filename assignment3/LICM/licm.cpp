// ECE 5984 S20 Assignment 3: licm.cpp
// Group: Ashwin & SengMing

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <dataflow.h>

using namespace llvm;

namespace {

/**
 * @brief Primary function pass to run LICM (Loop Invariant Code Motion) pass.
 * Prerequisite is dominators pass must have been run.
 */
class LICM : public FunctionPass {
      public:
        static char ID;
        LICM() : FunctionPass(ID) {}
        virtual bool runOnFunction(Function &F) { return true; }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {}
};

char LICM::ID = 0;
RegisterPass<LICM> X("licm", "ECE 5984 LICM Pass");
} // namespace
