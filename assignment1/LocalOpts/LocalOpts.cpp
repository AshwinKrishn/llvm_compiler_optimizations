// ECE-5984 S20 Assignment 1: LocalOpts.cpp
// PID: sengming
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

#define DEBUG_TYPE "local_opts"

#include <iostream>

using namespace llvm;

namespace {
class LocalOpts : public BasicBlockPass {
  protected:
    int m_numConstsFolded;
    int m_numStrengthReduced;

    // Member Optimization classes
    /**
     * @brief Handles Constant Folding optimization, subclass of instruction
     * visitor. Is a nested class of LocalOpts and thus is instantiated with a
     * reference to it for reporting.
     */
    class ConstantFold : public InstVisitor<ConstantFold> {
      protected:
        LocalOpts &m_localOpts;

      public:
        ConstantFold(LocalOpts &parent) : m_localOpts(parent){};
        void visitBinaryOperator(BinaryOperator &I) {
            // Operator visit
            DEBUG(dbgs() << I << "\n");
            int numConstOperands = I.getNumOperands();
            for (User::op_iterator iterator = I.op_begin(), end = I.op_end();
                 iterator != end; ++iterator) {
                if (isa<Constant>(*iterator))
                    --numConstOperands;
                outs() << I << " has operand: " << *iterator << "\n";
            }
            if (numConstOperands <= 0) {
                outs() << "All operands are constant"
                       << "\n";
            }
        }
    } m_constantFolder;

    /**
     * @brief Handles Strength Reduction optimization, subclass of instruction
     * visitor. Is a nested class of LocalOpts and thus is instantiated with a
     * reference to it for reporting.
     */
    class StrengthReduction : public InstVisitor<StrengthReduction> {
      protected:
        LocalOpts &m_localOpts;

      public:
        StrengthReduction(LocalOpts &parent) : m_localOpts(parent){};
        void visitBinaryOperator(BinaryOperator &I) {
            // Operator visit
            DEBUG(dbgs() << I << "\n");
            for (User::op_iterator op = I.op_begin(), end = I.op_end();
                 op != end; ++op) {
            }
        }
    } m_strengthReducer;

    /**
     * @brief Handles algebraic identity optimization, subclass of instruction
     * visitor. Is a nested class of LocalOpts and thus is instantiated with a
     * reference to it for reporting.
     */
    class AlgebraicIdentities : public InstVisitor<AlgebraicIdentities> {
      protected:
        LocalOpts &m_localOpts;

      public:
        AlgebraicIdentities(LocalOpts &parent) : m_localOpts(parent){};
        /**
         * @brief helper function to handle case where one of operands is a zero
         * value
         *
         * @param I: Instruction with zero operand
         * @param operandNumber: The operand which was zero
         */
        bool handleZeroVals(Instruction &I, unsigned int operandNumber) {
            bool retVal = false;
            switch (I.getOpcode()) {
            case Instruction::Mul:
            case Instruction::FMul:
                // If it's a multiply, we replace uses of it with zero, this is
                // for the case of 0*x or x*0
                DEBUG(dbgs()
                      << "Replaced 0 mults for Instruction: " << I << "\n");
                I.replaceAllUsesWith(I.getOperand(operandNumber));
                // I.eraseFromParent();
                retVal = true;
                break;
            case Instruction::Add:
            case Instruction::FAdd:
                // If it's an add, we just use the other operand and directly
                // replace, for the case of x + 0 or 0 + x
                DEBUG(dbgs()
                      << "Replaced 0 adds for Instruction: " << I << "\n");
                I.replaceAllUsesWith(I.getOperand(1 - operandNumber));
                // I.eraseFromParent();
                retVal = true;
                break;
            case Instruction::Sub:
            case Instruction::FSub:
                // If Operand is zero on the right, replace with nonzero,
                // similar to add, this is for the case of x-0
                DEBUG(dbgs()
                      << "Replaced 0 subs for Instruction: " << I << "\n");
                if (operandNumber == 1) {
                    I.replaceAllUsesWith(I.getOperand(1 - operandNumber));
                    // I.eraseFromParent();
                    retVal = true;
                    // For now we ignore the negative case since that identity
                    // is not in the test cases
                }
                break;
            }
            return retVal;
        }

        /**
         * @brief helper function to handle case where one of operands is a one
         * value
         *
         * @param I: Instruction with one operand
         * @param operandNumber: The operand which was one
         */
        bool handleOneVals(Instruction &I, unsigned int operandNumber) {
            bool retVal = false;
            switch (I.getOpcode()) {
            case Instruction::Mul:
            case Instruction::FMul:
                // If it's a multiply, we replace uses of it with itself
                DEBUG(dbgs()
                      << "Replaced 1 mults for Instruction: " << I << "\n");
                I.replaceAllUsesWith(I.getOperand(1 - operandNumber));
                // I.eraseFromParent();
                retVal = true;
                break;
            case Instruction::UDiv:
            case Instruction::SDiv:
            case Instruction::FDiv:
                DEBUG(dbgs()
                      << "Replaced 1 divisions for Instruction: " << I << "\n");
                // If Operand is one on the right, replace with the other
                // operand, for the case of x/1
                if (operandNumber == 1) {
                    I.replaceAllUsesWith(I.getOperand(1 - operandNumber));
                    // I.eraseFromParent();
                    retVal = true;
                }
                break;
            }
            return retVal;
        }

        /**
         * @brief helper function to handle cases where the operands are the
         * same
         *
         * @param I: Instruction to work on
         */
        bool handleEqualOpers(Instruction &I) {
            bool retVal = false;
            switch (I.getOpcode()) {
            case Instruction::Sub:
                // If it's a subtraction, we replace with constant 0, this is to
                // handle the case where x-x = 0
                DEBUG(dbgs()
                      << "Replaced eq subs for Instruction: " << I << "\n");
                I.replaceAllUsesWith(ConstantInt::get(I.getType(), 0));
                // I.eraseFromParent();
                retVal = true;
                break;
            case Instruction::FSub:
                // If it's a subtraction, we replace with constant 0, this is to
                // handle the case where x-x = 0.0
                DEBUG(dbgs()
                      << "Replaced eq fsubs for Instruction: " << I << "\n");
                I.replaceAllUsesWith(ConstantFP::get(I.getType(), 0.0));
                // I.eraseFromParent();
                retVal = true;
                break;
            case Instruction::UDiv:
            case Instruction::SDiv:
                // If it's a division, we replace with 1, this is to
                // handle the case where x/x = 1
                DEBUG(dbgs()
                      << "Replaced eq divs for Instruction: " << I << "\n");
                I.replaceAllUsesWith(ConstantInt::get(I.getType(), 1));
                // I.eraseFromParent();
                retVal = true;
                break;
            case Instruction::FDiv:
                // If Operand is one on the right, replace with the other
                // operand, for the case of x/1
                DEBUG(dbgs()
                      << "Replaced eq fdivs for Instruction: " << I << "\n");
                I.replaceAllUsesWith(ConstantFP::get(I.getType(), 1.0));
                // I.eraseFromParent();
                retVal = true;
                break;
            case Instruction::Add:
            case Instruction::FAdd:
                Constant *left_op, *right_op;
                if ((left_op = dyn_cast<Constant>(I.getOperand(0))) &&
                    (right_op = dyn_cast<Constant>(I.getOperand(1)))) {
                    if (left_op->isZeroValue() && right_op->isZeroValue()) {
                        I.replaceAllUsesWith(I.getOperand(0));
                        retVal = true;
                    }
                }
                break;
            }
            return retVal;
        }
        void visitBinaryOperator(BinaryOperator &I) {
            bool erase = false;
            // Operator visit
            DEBUG(dbgs() << I << "\n");
            // Equal Operands
            if (I.getOperand(0) == I.getOperand(1)) {
                erase = handleEqualOpers(I);
            } else {
                // Single operand specific
                for (User::op_iterator op = I.op_begin(), end = I.op_end();
                     op != end; ++op) {
                    // Cases we currently care about:
                    // +0, -0, *0, /1, *1, eq/eq, eq-eq
                    if (Constant *pconst_op = dyn_cast<Constant>(*op)) {
                        if (pconst_op->isZeroValue()) {
                            erase = handleZeroVals(I, op->getOperandNo());
                        } else if (pconst_op->isOneValue()) {
                            erase = handleOneVals(I, op->getOperandNo());
                        }
                    }
                }
            }
            if (erase)
                I.eraseFromParent();
        }
    } m_identityRemover;

  public:
    static char ID;
    LocalOpts()
        : BasicBlockPass(ID), m_numConstsFolded(0), m_numStrengthReduced(0),
          m_constantFolder(*this), m_strengthReducer(*this),
          m_identityRemover(*this) {}
    ~LocalOpts() {}

    void visitInstruction(Instruction &I) { DEBUG(dbgs() << I << "\n"); }
    // ReplaceInstWithInst(&I, );

    // Do some initialization
    bool doInitialization(Function &F) override { return false; }

    bool runOnBasicBlock(BasicBlock &B) override {
        // We perform optimization on the function local basic blocks
        m_identityRemover.visit(B);
        // m_strengthReducer.visit(B);
        // m_constantFolder.visit(B);
        return true;
    }
}; // namespace
} // namespace

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char LocalOpts::ID = 0;
static RegisterPass<LocalOpts> X("local_opts", "5984: Local Optimizations",
                                 false, false);
