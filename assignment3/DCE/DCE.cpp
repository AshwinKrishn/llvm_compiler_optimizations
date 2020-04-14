// ECE 5984 S20 Assignment 3: dce.cpp
// Group: Sengming & Ashwin

////////////////////////////////////////////////////////////////////////////////
#include "KillGen.h"
#include "MeetOpInterface.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
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
                        I->eraseFromParent();
                }

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
