// ECE 5984 S20 Assignment 3: available.cpp
// Group: Ashwin and SengMing

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
 * @brief Class to kill and gen doms list
 */
class KillGenDoms : public KillGen<BasicBlock *> {
      public:
        KillGenDoms() : KillGen() {}
        llvm::BitVector
        killEval(llvm::BasicBlock *BB, llvm::BitVector &meet_res,
                 std::vector<BasicBlock *> &domainset) override {
                llvm::BitVector BBkill(MAX_BITS_SIZE);
                // TODO: ADD IMPLEMENTATION
                // No Kill for Dominator pass
                BBkill.reset();
                return BBkill;
        }
        llvm::BitVector genEval(llvm::BasicBlock *BB, llvm::BitVector &meet_res,
                                std::vector<BasicBlock *> &domainset) override {
                llvm::BitVector BBgen(MAX_BITS_SIZE);
                // TODO: ADD IMPLEMENTATION
                std::vector<BasicBlock *>::iterator it =
                    std::find(domainset.begin(), domainset.end(), BB);
                BBgen.set((size_t)(it - domainset.begin()));
                return BBgen;
        }
};

/**
 * @brief Primary function pass to run Dominators pass.
 */
class DominatorsPass : public FunctionPass {
      public:
        static char ID;
        DominatorsPass() : FunctionPass(ID) {}

        virtual bool runOnFunction(Function &F) {
                // Start of our modifications

                // Instantiate requirements
                IntersectionMeet intersect;
                KillGenDoms killGenDom;
                llvm::DenseMap<BasicBlock *, std::bitset<4096>> base_tree;
                std::vector<BasicBlock *> bb_ids;
                for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE;
                     ++FI) {
                        BasicBlock *block = &*FI;
                        bb_ids.push_back(block);
                }
                BaseTransferFunction transferFunc;
                DataflowFramework<BasicBlock *> DF(intersect, FORWARD, EMPTY, F,
                                                   bb_ids, killGenDom,
                                                   transferFunc);
                llvm::DenseMap<BasicBlock *, BBInOutBits *> *DomTree = DF.run();
                for (auto it = DomTree->begin(); it != DomTree->end(); ++it) {
                        outs() << "Basic Block" << *(it->first) << "\n";
                }

                // Did not modify the incoming Function.
                return false;
        }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesAll();
        }
};

char DominatorsPass::ID = 0;
RegisterPass<DominatorsPass> X("dominators", "ECE 5984 Dominators");
} // namespace
