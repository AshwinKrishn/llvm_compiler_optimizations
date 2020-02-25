// ECE-5984 S20 Assignment 1: FunctionInfo.cpp
// PID:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Pass.h"
#include "llvm/Support/Debug.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

#define DEBUG_TYPE "func_info"

using namespace llvm;
namespace {
class FunctionInfo : public FunctionPass, public InstVisitor<FunctionInfo> {
  private:
    struct InstructionInfo {
        uint64_t numAdds;
        uint64_t numSubs;
        uint64_t numMult;
        uint64_t numDiv;
        uint64_t numConditionalBr;
        uint64_t numUncondBr;
        uint64_t totalInsts;
    };
    Module *m_pmodule;
    Function *m_pfunction;
    int m_numCallsToFunc;

  public:
    static char ID;
    FunctionInfo() : FunctionPass(ID), m_numCallsToFunc(0) {}
    ~FunctionInfo() {}

    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
        AU.setPreservesAll();
    }

    // Do some initialization
    bool doInitialization(Module &M) override {
        errs() << "5984 Function Information Pass\n"; // TODO: remove this.
        outs() << "Name,\tArgs,\tCalls,\tBlocks,\tInsns,\tAdd/Sub,\tMul/"
                  "Div,\tBr(Cond),\tBr(UnCond)"
               << "\n";
        m_pmodule = &M;
        return false;
    }

    // Helper function to dump instruction information
    void dumpInstructionInfo(struct InstructionInfo &I, Function &F) {
        I.totalInsts = 0;
        for (inst_iterator it = inst_begin(F), end = inst_end(F); it != end;
             ++it, ++I.totalInsts) {
            switch ((*it).getOpcode()) {
            case Instruction::Add:
                ++I.numAdds;
                break;
            case Instruction::Sub:
                ++I.numSubs;
                break;
            case Instruction::Mul:
            case Instruction::FMul:
                ++I.numMult;
                break;
            case Instruction::UDiv:
            case Instruction::SDiv:
            case Instruction::FDiv:
                ++I.numDiv;
                break;
            case Instruction::Br:
            case Instruction::IndirectBr:
                BranchInst *brinst;
                if ((brinst = dyn_cast<BranchInst>(&*it))) {
                    if (brinst->isConditional())
                        ++I.numConditionalBr;
                    else
                        ++I.numUncondBr;
                }
                break;
            }
        }
        return;
    }

    void visitCallInst(CallInst &I) {
        // Check if call references us
        if (I.getCalledFunction() == m_pfunction) {
            // We are the call target
            DEBUG(dbgs() << "call inst:" << I << "\n");
            ++m_numCallsToFunc;
        }
    }
    // Print output for each function
    bool runOnFunction(Function &F) override {
        outs() << F.getName() << ",   ";
        // Number of arguments
        if (F.isVarArg())
            outs() << "*, \t";
        else
            outs() << F.arg_size() << ",\t";
        // Number of direct call sites if we have the information
        if (F.hasProfileData()) {
            outs() << F.getEntryCount().getValueOr(0) << ",\t";
        } else {
            // We are an instruction visitor, visit the module's instructions
            m_pfunction = &F;
            visit(*m_pmodule);
            outs() << m_numCallsToFunc << ",\t";
        }
        // Number of basic blocks
        outs() << F.getBasicBlockList().size() << ",\t";
        struct InstructionInfo ii = {0};
        dumpInstructionInfo(ii, F);
        outs() << ii.totalInsts << ",\t";
        outs() << ii.numAdds + ii.numSubs << ",\t";
        outs() << ii.numMult + ii.numDiv << ",\t";
        outs() << ii.numConditionalBr << ",\t";
        outs() << ii.numUncondBr << "\n";

        return false;
    }
};
} // namespace

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char FunctionInfo::ID = 0;
static RegisterPass<FunctionInfo> X("function-info",
                                    "5984: Function Information", false, false);
