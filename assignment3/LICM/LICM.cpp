// ECE 5984 S20 Assignment 3: licm.cpp
// Group: Ashwin & SengMing

////////////////////////////////////////////////////////////////////////////////
// llvm specific
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

// Framework and ours
#include <DominatorPass.h>
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <ReachingDefinitions.h>
#include <dataflow.h>
using namespace llvm;

namespace {

/**
 * @brief Primary function pass to run LICM (Loop Invariant Code Motion) pass.
 * Prerequisite is dominators pass and RD pass must have been run. This is just
 * currently for testing, we will have to change this to a loop pass later
 */
class LICM : public FunctionPass {
      public:
        static char ID;
        LICM() : FunctionPass(ID) {}
        virtual bool runOnFunction(Function &F) {
                DenseMap<BasicBlock *, BBInOutBits *> *reachingDefs =
                    getAnalysis<ReachingDefsPass>().getRDResults();
                DenseMap<BasicBlock *, BBInOutBits *> *DomResult =
                    getAnalysis<DominatorsPass>().getDomResults();
                for (DenseMap<BasicBlock *, BBInOutBits *>::iterator rdbegin =
                         reachingDefs->begin();
                     rdbegin != reachingDefs->end(); ++rdbegin) {
                        BBInOutBits::printBitVector(rdbegin->second->m_OUT,
                                                    MAX_PRINT_SIZE);
                }
                // Compute loop invariant statements
                // getLoopInvariantStatements();
                // checkConditionForCodeMotion();

                return true;
        }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesCFG();
                AU.addRequired<ReachingDefsPass>();
                AU.addRequired<DominatorsPass>();
        }
};

char LICM::ID = 1;
RegisterPass<LICM> licm("licm_pass", "ECE 5984 LICM Pass");
} // namespace
