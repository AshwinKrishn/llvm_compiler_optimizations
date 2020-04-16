// ECE 5984 S20 Assignment 3: LandingPadTransform.cpp
// Group: Ashwin and SengMing

////////////////////////////////////////////////////////////////////////////////

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

// ours
#include <IntersectionMeet.h>
#include <LandingPadTransform.h>
#include <dataflow.h>

// llvm specific
#include <llvm/Analysis/LoopPass.h>

namespace llvm {

LandingPadTransform::LandingPadTransform() : LoopPass(ID) {}

bool LandingPadTransform::runOnLoop(Loop *L, LPPassManager &LPM) {
        BasicBlock *preheader = L->getLoopPreheader();
        BasicBlock *header = L->getHeader();
        // Function *parentFunc = preheader->getParent();
        // If eligible for transform, add landing pad
        SmallVector<BasicBlock *, 2> newBBs;
        if (preheader) {
                // preheader->insertInto
                // llvm::SplitBlock(preheader, preheader->getTerminator());
                //       llvm::SplitLandingPadPredecessors(header, preheader,
                //       "lp1",
                //                                       "lp2", newBBs);
        }

        // outs() << *(L->getLoopPreheader()) << "\n";

        return true;
}

void LandingPadTransform::getAnalysisUsage(AnalysisUsage &AU) const {
        // AU.setPreservesAll();
        // AU.setPreservesCFG();
}

char LandingPadTransform::ID = 2;
RegisterPass<LandingPadTransform> lpadtrans("landing-pad",
                                            "ECE 5984 Landing Pad");
} // namespace llvm
