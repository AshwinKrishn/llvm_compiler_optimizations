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
        std::bitset<MAX_BITS_SIZE>
        killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                 std::vector<BasicBlock *> &domainset) override {
                std::bitset<MAX_BITS_SIZE> BBkill;
                // TODO: ADD IMPLEMENTATION

                BBkill.reset();
                return BBkill;
        }
        std::bitset<MAX_BITS_SIZE>
        genEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                std::vector<BasicBlock *> &domainset) override {
                std::bitset<MAX_BITS_SIZE> BBgen;
                // TODO: ADD IMPLEMENTATION

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
                KillGenDoms killGenDoms;
                BaseTransferFunction transferFunc;
                // DataflowFramework<Expression> DF(intersect, FORWARD, EMPTY,
                // F,
                //                                 expressions, KillGenAE,
                //                                 transferFunc);
                // DF.run();

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
