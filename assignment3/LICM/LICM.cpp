// ECE 5984 S20 Assignment 3: licm.cpp
// Group: Ashwin & SengMing

////////////////////////////////////////////////////////////////////////////////
// llvm specific
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

// Framework and ours
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <ReachingDefinitions.h>
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
        virtual bool runOnFunction(Function &F) {
                DenseMap<BasicBlock *, BBInOutBits *> *reachingDefs =
                    getAnalysis<ReachingDefsPass>().getRDResults();
                for (DenseMap<BasicBlock *, BBInOutBits *>::iterator rdbegin =
                         reachingDefs->begin();
                     rdbegin != reachingDefs->end(); ++rdbegin) {
                        outs() << rdbegin->second->m_OUT.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE)
                               << "\n";
                }
                return true;
        }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesCFG();
                AU.addRequired<ReachingDefsPass>();
        }
};

char LICM::ID = 1;
RegisterPass<LICM> licm("licm_pass", "ECE 5984 LICM Pass");
} // namespace
