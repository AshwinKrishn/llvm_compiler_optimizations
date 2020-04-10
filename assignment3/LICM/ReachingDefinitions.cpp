// ECE 5984 S20 Assignment 3: ReachingDefinitions.cpp
// Group: Ashwin and SengMing

////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <ReachingDefinitions.h>

using namespace llvm;

llvm::BitVector KillGenRD::killEval(llvm::BasicBlock *BB,
                                    llvm::BitVector &meet_res,
                                    std::vector<Value *> &domainset) {
        llvm::BitVector BBkill(MAX_BITS_SIZE);
        // In SSA, no definitions will get killed because there is never
        // a redefinition
        BBkill.reset();
        return BBkill;
}

llvm::BitVector KillGenRD::genEval(llvm::BasicBlock *BB,
                                   llvm::BitVector &meet_res,
                                   std::vector<Value *> &domainset) {
        llvm::BitVector BBgen(MAX_BITS_SIZE);
        // We generate a definition when we see it
        for (Instruction &I : *BB) {
                if (isa<BinaryOperator>(I) || isa<PHINode>(I)) {
                        std::vector<Value *>::iterator it =
                            std::find(domainset.begin(), domainset.end(), &I);
                        BBgen.set((size_t)(it - domainset.begin()));
                }
        }

        return BBgen;
}

ReachingDefsPass::ReachingDefsPass() : FunctionPass(ID) {}

bool ReachingDefsPass::runOnFunction(Function &F) {

        std::vector<Value *> definitions;

        // Find definitions either through PHI nodes or binary operators
        // and add to list
        for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                        if (isa<BinaryOperator>(I) || isa<PHINode>(I)) {
                                definitions.push_back(&I);
                        }
                }
        }

        // Instantiate requirements
        UnionMeet unionMeet;
        KillGenRD killGenRD;
        BaseTransferFunction transferFunc;
        DataflowFramework<Value *> DF(unionMeet, FORWARD, EMPTY, F, definitions,
                                      killGenRD, transferFunc);
        m_rdresults = DF.run();

        // Did not modify the incoming Function.
        return false;
}

void ReachingDefsPass::getAnalysisUsage(AnalysisUsage &AU) const {
        AU.setPreservesAll();
}

DenseMap<BasicBlock *, BBInOutBits *> *ReachingDefsPass::getRDResults() {
        return m_rdresults;
}

char ReachingDefsPass::ID = 0;
RegisterPass<ReachingDefsPass> rd("rd", "ECE 5984 Reaching Definitions");
