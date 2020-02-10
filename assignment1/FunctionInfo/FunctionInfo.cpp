// ECE-5984 S20 Assignment 1: FunctionInfo.cpp
// PID:
////////////////////////////////////////////////////////////////////////////////

#include "llvm/IR/Function.h"
#include "llvm/IR/InstIterator.h"
#include "llvm/IR/InstVisitor.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

#include <iostream>

using namespace llvm;
namespace {
class FunctionInfo : public FunctionPass {
  private:
    struct InstructionInfo {
        uint64_t numAdds;
        uint64_t numSubs;
        uint64_t numMult;
        uint64_t numDiv;
        uint64_t numBranch;
    };

  public:
    static char ID;
    FunctionInfo() : FunctionPass(ID) {}
    ~FunctionInfo() {}
    // We don't modify the program, so we preserve all analyses
    void getAnalysisUsage(AnalysisUsage &AU) const override {
        AU.setPreservesAll();
    }

    // Do some initialization
    bool doInitialization(Module &M) override {
        errs() << "5984 Function Information Pass\n"; // TODO: remove this.
        // outs() <<
        // "Name,\tArgs,\tCalls,\tBlocks,\tInsns\n,\tAdd/Sub,\tMul/Div,\tBr(Cond),\tBr(UnCond)";

        return false;
    }

    // Helper function to dump instruction information
    void dumpInstructionInfo(struct InstructionInfo &I, Function &F) {
        for (inst_iterator it = inst_begin(F), end = inst_end(F); it != end;
             ++it) {
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
                ++I.numBranch;
                break;
            }
        }
        return;
    }

    // Print output for each function
    bool runOnFunction(Function &F) override {
        // outs() << "name" << ",\t" << "args" << ",\t" << "calls" <<
        // ",\t" << "bbs" << ",\t" << "insts ...." << "\n";
        outs() << "Inside Function: " << F.getName() << "\n";
        // Number of arguments
        outs() << "Number of Arguments: ";
        F.isVarArg() ? outs() << "*"
                              << "\n"
                     : outs() << F.arg_size() << "\n";
        // Number of direct call sites
        if (F.hasProfileData()) {
            outs() << "Number of calls to function: "
                   << F.getEntryCount().getValueOr(0) << "\n";
        }
        // Number of basic blocks
        outs() << "Number of basic blocks: " << F.getBasicBlockList().size()
               << "\n";
        struct InstructionInfo ii = {0};
        dumpInstructionInfo(ii, F);
        outs() << "Number of adds: " << ii.numAdds << "\n";
        outs() << "Number of subs: " << ii.numSubs << "\n";
        outs() << "Number of mults: " << ii.numMult << "\n";
        outs() << "Number of divs: " << ii.numDiv << "\n";
        outs() << "Number of branches: " << ii.numBranch << "\n";

        return false;
    }
};
} // namespace

// LLVM uses the address of this static member to identify the pass, so the
// initialization value is unimportant.
char FunctionInfo::ID = 0;
static RegisterPass<FunctionInfo> X("function-info",
                                    "5984: Function Information", false, false);
