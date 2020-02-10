// ECE-5984 S20 Assignment 1: LocalOpts.cpp
// PID:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Instrumentation.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"

#include <iostream>

using namespace llvm;

namespace {
class LocalOpts : public BasicBlockPass, public InstVisitor<LocalOpts> {
  protected:
    int m_numConstsFolded;
    int m_numStrengthReduced;

    // class OperandChecker : public InstVisitor<OperandChecker> {
    //} m_operandChecker;

    // int foldConstants(Instruction &ins, BasicBlock &BB) {
    //    for (User::op_iterator iterator = ins.op_begin(), end = ins.op_end();
    //         iterator != end; ++iterator) {
    //        m_operandChecker.visit(BB);
    //    }
    //    return 0;
    //}

  public:
    static char ID;
    LocalOpts()
        : BasicBlockPass(ID), m_numConstsFolded(0), m_numStrengthReduced(0) {}
    ~LocalOpts() {}

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
        AU.setPreservesAll();
    }

    void visitBinaryOperator(BinaryOperator &I) {
        // Operator visit
        dbgs() << I << "\n";
        for (User::op_iterator iterator = I.op_begin(), end = I.op_end();
             iterator != end; ++iterator) {
            if (ConstantInt *val = dyn_cast<llvm::ConstantInt>(iterator)) {
                outs() << I << " has constant: " << val->getValue() << "\n";
            }
        }
        // ReplaceInstWithInst(&I, );
        return;
    }

    // Do some initialization
    bool doInitialization(Function &F) override { return false; }

    bool runOnBasicBlock(BasicBlock &B) override {
        // We perform optimization on the function local basic blocks
        visit(B);
        return false;
    }
};
} // namespace

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
static RegisterPass<LocalOpts> X("local_opts", "5984: Local Optimizations",
                                 false, false);
