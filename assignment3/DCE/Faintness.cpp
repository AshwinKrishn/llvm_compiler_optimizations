// ECE 5984 S20 Assignment 3: faintness.cpp
// Group: Ashwin and SengMing

////////////////////////////////////////////////////////////////////////////////
#include <Faintness.h>

llvm::BitVector KillGenFaint::killEval(llvm::BasicBlock *BB,
                                       llvm::BitVector &meet_res,
                                       std::vector<Value *> &domainset) {
        llvm::BitVector BBkill(MAX_BITS_SIZE);

        BBkill.reset();
        return BBkill;
}

llvm::BitVector KillGenFaint::genEval(llvm::BasicBlock *BB,
                                      llvm::BitVector &meet_res,
                                      std::vector<Value *> &domainset) {
        llvm::BitVector BBgen(MAX_BITS_SIZE);
        // TODO: ADD IMPLEMENTATION
        // We only need a constgen, so no need for helpers, calculate in here
        return BBgen;
}

void KillGenFaint::setBitsIfInDomain(Value *V, llvm::BitVector &bits,
                                     std::vector<Value *> &domainset) {
        std::vector<Value *>::iterator it =
            std::find(domainset.begin(), domainset.end(), V);

        if (it != domainset.end()) {
                bits.set((size_t)(it - domainset.begin()));
        }
}

llvm::BitVector KillGenFaint::constKillHelper(llvm::BasicBlock *BB,
                                              llvm::BitVector &meet_res,
                                              std::vector<Value *> &domainset) {
        // Iterate through instructions, if they are users of of a value in the
        // domainset AND they are non assignment statements, then kill. If they
        // are br instructions for instance (which are terminator instructions),
        // if they are debug info, or landingpad instructions
        llvm::BitVector constKillSet(MAX_BITS_SIZE);
        constKillSet.reset();
        for (Instruction &I : *BB) {
                if (isa<TerminatorInst>(I) || isa<LandingPadInst>(I) ||
                    I.mayHaveSideEffects()) {
                        for (auto OP = I.op_begin(); OP != I.op_end(); ++OP) {
                                setBitsIfInDomain(OP->get(), constKillSet,
                                                  domainset);
                        }
                }
        }

        return constKillSet;
}

llvm::BitVector depKillHelper(llvm::BasicBlock *BB, llvm::BitVector &meet_res,
                              std::vector<Value *> &domainset) {

        llvm::BitVector depKillSet(MAX_BITS_SIZE);
        depKillSet.reset();
        // If we are an assignment statement (either binary op or phi node) AND
        // x not in OUT meet_res holds the bits that are entering, so they are
        // OUT.
        for (Instruction &I : *BB) {
                if (isa<BinaryOperator>(I) || isa<PHINode>(I)) {
                }
        }

        return depKillSet;
}

FaintnessPass::FaintnessPass() : FunctionPass(ID) {}

bool FaintnessPass::runOnFunction(Function &F) {
        // Start of our modifications
        std::vector<Value *> variables;

        // For now we only add binary op instructions and phi nodes as variables
        for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                        if (isa<BinaryOperator>(I) || isa<PHINode>(I)) {
                                variables.push_back(&I);
                        }
                }
        }

        // Instantiate requirements
        IntersectionMeet intersect;
        KillGenFaint killGenFaint;
        BaseTransferFunction transferFunc;
        DataflowFramework<Value *> DF(intersect, BACKWARD, UNIVERSAL, F,
                                      variables, killGenFaint, transferFunc);
        DF.run();

        // Did not modify the incoming Function.
        return false;
}

void FaintnessPass::getAnalysisUsage(AnalysisUsage &AU) const {
        AU.setPreservesAll();
}

char FaintnessPass::ID = 0;
RegisterPass<FaintnessPass> X("faintness", "ECE 5984 Faint Analysis Pass");
