// ECE 5984 S20 Assignment 3: dce.cpp
// Group: Sengming & Ashwin

////////////////////////////////////////////////////////////////////////////////
#include "KillGen.h"
#include "MeetOpInterface.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <llvm/IR/Constants.h>
#include <llvm/Transforms/Utils/BasicBlockUtils.h>

#include <Faintness.h>
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <dataflow.h>
#include <vector>

using namespace llvm;

namespace {

/**
 * @brief Dead code elimintation function pass
 */
class DCE : public FunctionPass {
      public:
        static char ID;
        DCE() : FunctionPass(ID) {}

        /**
         * @brief Checks if a given value is present in the bitvector. We need
         * to know the domainset because the bitvector follows its ordering and
         * it contains the values.
         *
         * @param V
         * @param bits
         * @param domainset
         *
         * @return
         */
        bool isValueInBits(const Value *V, const llvm::BitVector &bits,
                           const std::vector<Value *> &domainset) {
                bool retVal = false;
                for (int bitIndex : bits.set_bits()) {
                        if (bitIndex < (int)domainset.size()) {
                                if (V == domainset[bitIndex])
                                        retVal = true;
                        }
                }
                return retVal;
        }

        /**
         * @brief Experimental function, attempt to fold direct unconditional
         * branches and save br instructions
         *
         * @param F
         */
        void foldDirectBranches(Function &F) {
                std::vector<BasicBlock *> removeList;
                for (BasicBlock &BB : F) {
                        // If the basic block has one instruction and that is an
                        // unconditional branch with 1 successor and 1
                        // predecessor
                        if (BB.size() == 1) {
                                if (BranchInst *I =
                                        dyn_cast<BranchInst>(&BB.front())) {
                                        outs() << BB << "\n";
                                        BasicBlock *pred =
                                            BB.getSinglePredecessor();
                                        if (I->getNumSuccessors() == 1 &&
                                            pred) {
                                                // Replace downstream phi node
                                                // successor uses with our
                                                // predecessor
                                                BB.replaceSuccessorsPhiUsesWith(
                                                    pred);
                                                BranchInst *prev_term =
                                                    cast<BranchInst>(
                                                        pred->getTerminator());
                                                if (prev_term
                                                        ->isUnconditional()) {
                                                        prev_term->setOperand(
                                                            0,
                                                            BB.getSingleSuccessor());
                                                }
                                                removeList.push_back(&BB);
                                        }
                                }
                        }
                }
                for (BasicBlock *BB : removeList) {
                        // BB->eraseFromParent();
                        llvm::MergeBlockIntoPredecessor(BB);
                }
        }

        virtual bool runOnFunction(Function &F) {
                DenseMap<BasicBlock *, std::vector<BitVector> *> *faintMap =
                    getAnalysis<FaintnessPass>().getFaintResults();

                std::vector<Value *> domainSet =
                    getAnalysis<FaintnessPass>().getDomainSet();

                DBUG(for (Value *V : domainSet) { outs() << *V << "\n"; });

                outs() << "Running DCE on function: " << F.getName() << "\n";

                std::vector<Instruction *> removeSet;

                for (BasicBlock &BB : F) {
                        DBUG(outs() << BB << "\n"; outs() << "Instructions: "
                                                          << "\n";);
                        unsigned long instructionNo = 0;
                        std::vector<BitVector> *outBits = (*faintMap)[&BB];

                        for (Instruction &I : BB) {
                                DBUG(outs() << I << "\n";
                                     BBInOutBits::printBitVector(
                                         (*outBits)[instructionNo],
                                         MAX_PRINT_SIZE););

                                // If we're not a terminator instruction and if
                                // we are faint right after definition, we can
                                // be removed.
                                if (!isa<TerminatorInst>(I) &&
                                    isValueInBits(&I, (*outBits)[instructionNo],
                                                  domainSet)) {
                                        removeSet.push_back(&I);
                                }

                                ++instructionNo;
                        }
                        DBUG(outs() << "--------------------------------"
                                    << "\n";);
                }

                // Remove all faint Instructions:
                for (Instruction *I : removeSet) {
                        outs() << "We can remove: " << *I << "\n";
                        UndefValue *undef = UndefValue::get(I->getType());
                        I->replaceAllUsesWith(undef);
                        I->eraseFromParent();
                }

                // If we can fix this function in time, then add it, else keep
                // commented.
                // foldDirectBranches(F);
                return true;
        }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesCFG();
                AU.addRequired<FaintnessPass>();
        }

      private:
};

char DCE::ID = 1;
RegisterPass<DCE> X("dce_pass", "ECE 5984 Dead Code Elimination");
} // namespace
