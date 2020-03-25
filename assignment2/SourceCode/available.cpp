// ECE 5984 S20 Assignment 2: available.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////

#include "available-support.h"
#include "dataflow.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>

using namespace llvm;
using namespace std;

namespace {

class KillGenEval : public KillGen<Expression> {
  public:
    KillGenEval() : KillGen() {}
    std::bitset<MAX_BITS_SIZE>
    killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &list,
             std::bitset<MAX_BITS_SIZE> &depset,
             std::vector<Expression> &domainset) override {
        for (Instruction &I : *BB) {
            // null for Available expression
        }
        return list;
    }
    std::bitset<MAX_BITS_SIZE>
    genEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &list,
            std::bitset<MAX_BITS_SIZE> &depset,
            std::vector<Expression> &domainset) override {
        for (Instruction &I : *BB) {

            //			set the bit for corresponding
            // expression;
            //			ret = in.find(Expression);
            //			list[ret] = 1;
        }
        return list;
    }
};
class AvailableExpressions : public FunctionPass {
  public:
    static char ID;
    AvailableExpressions() : FunctionPass(ID) {}
    virtual bool runOnFunction(Function &F) {

        // Here's some code to familarize you with the Expression
        // class and pretty printing code we've provided:

        vector<Expression> expressions;
        for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE; ++FI) {
            BasicBlock *block = &*FI;
            for (BasicBlock::iterator i = block->begin(), e = block->end();
                 i != e; ++i) {

                Instruction *I = &*i;
                // We only care about available expressions for
                // BinaryOperators
                if (BinaryOperator *BI = dyn_cast<BinaryOperator>(I)) {

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
        DataflowFramework<Expression> DF(intersect, FORWARD, F, expressions,
                                         KillGenAE);
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
