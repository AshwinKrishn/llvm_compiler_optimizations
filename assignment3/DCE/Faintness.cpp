// ECE 5984 S20 Assignment 3: faintness.cpp
// Group: Ashwin and SengMing

////////////////////////////////////////////////////////////////////////////////
#include <Faintness.h>

llvm::BitVector KillGenFaint::killEval(llvm::BasicBlock *BB,
                                       llvm::BitVector &meet_res,
                                       std::vector<Value *> &domainset) {
        // constkill U depKill is the kill
        llvm::BitVector BBKill = constKillHelper(BB, meet_res, domainset);
        BBKill |= depKillHelper(BB, meet_res, domainset);
        return BBKill;
}

llvm::BitVector KillGenFaint::genEval(llvm::BasicBlock *BB,
                                      llvm::BitVector &meet_res,
                                      std::vector<Value *> &domainset) {
        llvm::BitVector BBgen(MAX_BITS_SIZE);
        // We only need a constgen, so no need for helpers, calculate in here
        for (Instruction &I : *BB) {
                if (isa<BinaryOperator>(I) || isa<PHINode>(I)) {
                        // If A is of the form x = e and x!= oper(e), gen it.
                        // In SSA, we will never be in our own operand list,
                        // single definition, so, just gen if you see
                        setBitsIfInDomain(&I, BBgen, domainset);
                }
        }

        return BBgen;
}

void KillGenFaint::setBitsIfInDomain(const Value *V, llvm::BitVector &bits,
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
                // TODO: Check if correct, we assume first operand for br,
                // indbr, switch, etc. is the target
                if (isa<TerminatorInst>(I) || isa<LandingPadInst>(I) ||
                    I.mayHaveSideEffects()) {
                        setBitsIfInDomain(I.getOperand(0), constKillSet,
                                          domainset);
                }
        }

        return constKillSet;
}

bool KillGenFaint::isValueInOUT(const Value *V, const llvm::BitVector &OUT,
                                const std::vector<Value *> &domainset) {
        bool retVal = false;
        for (int bitIndex : OUT.set_bits()) {
                if (V == domainset[bitIndex])
                        retVal = true;
        }
        return retVal;
}

llvm::BitVector KillGenFaint::depKillHelper(llvm::BasicBlock *BB,
                                            llvm::BitVector &meet_res,
                                            std::vector<Value *> &domainset) {

        llvm::BitVector depKillSet(MAX_BITS_SIZE);
        depKillSet.reset();
        // If we are an assignment statement (either binary op or phi node) AND
        // x not in OUT. meet_res holds the bits that are entering, so they are
        // OUT.
        for (Instruction &I : *BB) {
                if (isa<BinaryOperator>(I) || isa<PHINode>(I)) {
                        // If the LHS is not in OUT, then that e's faintness was
                        // killed in the BB. Thus, add operands of e as killed.
                        if (isValueInOUT(&I, meet_res, domainset)) {
                                for (auto OP = I.op_begin(); OP != I.op_end();
                                     ++OP) {
                                        setBitsIfInDomain(OP->get(), depKillSet,
                                                          domainset);
                                }
                        }
                }
        }

        return depKillSet;
}

FaintnessPass::FaintnessPass() : FunctionPass(ID) {}

bool FaintnessPass::runOnFunction(Function &F) {
        // Start of our modifications
        std::vector<Value *> variables;
        outs() << "Faintness for Function: " << F.getName() << "\n";

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
        m_faintness = DF.run();

        // Did not modify the incoming Function.
        return false;
}

void FaintnessPass::getAnalysisUsage(AnalysisUsage &AU) const {
        AU.setPreservesAll();
}

llvm::DenseMap<BasicBlock *, BBInOutBits *> *FaintnessPass::getFaintResults() {
        return m_faintness;
}

char FaintnessPass::ID = 0;
RegisterPass<FaintnessPass> X("faintness", "ECE 5984 Faint Analysis Pass");
