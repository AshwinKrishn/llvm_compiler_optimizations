// ECE 5984 S20 Assignment 3: licm.cpp
// Group: Ashwin & SengMing

////////////////////////////////////////////////////////////////////////////////
// llvm specific
#include "llvm/Analysis/LoopInfo.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/Analysis/LoopPass.h>
// Framework and ours
#include <DominatorPass.h>
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <ReachingDefinitions.h>
#include <dataflow.h>

namespace llvm {

/**
 * @brief Primary function pass to run LICM (Loop Invariant Code Motion) pass.
 * Prerequisite is dominators pass and RD pass must have been run. This is just
 * currently for testing, we will have to change this to a loop pass later
 */
class LICM : public LoopPass {
      public:
        static char ID;
        LoopInfo *LI;
        DenseMap<BasicBlock *, BBInOutBits *> *reachingDefs;
        DenseMap<BasicBlock *, BBInOutBits *> *DomResult;
        LICM() : LoopPass(ID) {}
        virtual bool runOnLoop(Loop *L, LPPassManager &LPM) {
                reachingDefs = getAnalysis<ReachingDefsPass>().getRDResults();
                DomResult = getAnalysis<DominatorsPass>().getDomResults();
                outs() << "Basic Blocks in the loop \n\n";
                for (Loop::block_iterator i = L->block_begin(),
                                          e = L->block_end();
                     i != e; ++i) {
                        BasicBlock *BB = *i;
                        outs() << *BB << "\n\n";
                        /*  if (LI->getLoopFor(BB) == L) {
                                  outs() << *BB << "\n\n";
                          }
                          */
                }

                // Compute loop invariant statements
                // getLoopInvariantStatements();
                // checkConditionForCodeMotion();
                return false;
        }
        virtual bool doInitialization(Loop *, LPPassManager &LPM) {
                /* for (DenseMap<BasicBlock *, BBInOutBits *>::iterator rdbegin
                 = reachingDefs->begin(); rdbegin != reachingDefs->end();
                 ++rdbegin) {
                         BBInOutBits::printBitVector(rdbegin->second->m_OUT,
                                                     MAX_PRINT_SIZE);
                 }
 */
                return true;
        }
        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesCFG();
                AU.addRequired<ReachingDefsPass>();
                AU.addRequired<DominatorsPass>();
                AU.addRequired<LoopInfoWrapperPass>();
        }
};

char LICM::ID = 1;
RegisterPass<LICM> licm("licm_pass", "ECE 5984 LICM Pass");
} // namespace llvm
