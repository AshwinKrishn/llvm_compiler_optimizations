// ECE 5984 S20 Assignment 2: available.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////

#include "available-support.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <dataflow.h>

using namespace llvm;
using namespace std;

#define DEBUG_TYPE "dataflow_framework"

namespace {

/**
 * @brief KillGenEval is a subclass of KillGen class, which is a template class.
 * Main function of this class is to provide a killEval and genEval function to
 * take in the input bit set which is the result of the meet operator, the
 * current Basic Block, and the domainset, which is a vector of objects we want
 * to perform the analysis on (Expression in this case) and returns the
 * resultant kill set or gen set bits respectively.
 */
class KillGenEval : public KillGen<Expression> {
      public:
        KillGenEval() : KillGen() {}
        std::bitset<MAX_BITS_SIZE>
        killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                 std::vector<Expression> &domainset) override {
                std::bitset<MAX_BITS_SIZE> BBkill;
                for (Instruction &I : *BB) {
                        // null for Available expression long live SSA
                }
                BBkill.reset();
                return BBkill;
        }
        std::bitset<MAX_BITS_SIZE>
        genEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                std::vector<Expression> &domainset) override {
                std::bitset<MAX_BITS_SIZE> BBgen;
                for (Instruction &I : *BB) {
                        if (BinaryOperator *BI = dyn_cast<BinaryOperator>(&I)) {
                                Expression Ins = Expression(&I);
                                std::vector<Expression>::iterator it =
                                    std::find(domainset.begin(),
                                              domainset.end(), Ins);
                                BBgen.set((size_t)(it - domainset.begin()));
                        }
                }
                return BBgen;
        }
};

/**
 * @brief Primary function pass to run AvailableExpressions pass.
 */
class AvailableExpressions : public FunctionPass {
      public:
        static char ID;
        AvailableExpressions() : FunctionPass(ID) {}
        virtual bool runOnFunction(Function &F) {

                // Here's some code to familarize you with the Expression
                // class and pretty printing code we've provided:

                vector<Expression> expressions;
                for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE;
                     ++FI) {
                        BasicBlock *block = &*FI;
                        for (BasicBlock::iterator i = block->begin(),
                                                  e = block->end();
                             i != e; ++i) {

                                Instruction *I = &*i;
                                // We only care about available expressions for
                                // BinaryOperators
                                if (BinaryOperator *BI =
                                        dyn_cast<BinaryOperator>(I)) {

                                        expressions.push_back(Expression(BI));
                                }
                        }
                }

                // Print out the expressions used in the function
                outs() << "Expressions used by this function:\n";
                printSet(&expressions);

                // Start of our modifications

                // Instantiate requirements
                IntersectionMeet intersect;
                KillGenEval KillGenAE;
                BaseTransferFunction transferFunc;
                DataflowFramework<Expression> DF(intersect, FORWARD, EMPTY, F,
                                                 expressions, KillGenAE,
                                                 transferFunc);
                DF.run();

                // Did not modify the incoming Function.
                return false;
        }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesAll();
        }
};

char AvailableExpressions::ID = 0;
RegisterPass<AvailableExpressions> X("available",
                                     "ECE 5984 Available Expressions");
} // namespace
