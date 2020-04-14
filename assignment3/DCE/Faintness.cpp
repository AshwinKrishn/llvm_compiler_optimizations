// ECE 5984 S20 Assignment 3: faintness.cpp
// Group: Ashwin and SengMing

////////////////////////////////////////////////////////////////////////////////
#include <Faintness.h>
#include <llvm/ADT/DenseMap.h>
#include <llvm/IR/InstrTypes.h>
#include <llvm/IR/Instructions.h>
#include <vector>

FaintTransferFunction::FaintTransferFunction(
    llvm::DenseMap<BasicBlock *, std::vector<llvm::BitVector> *>
        &bbToInstOUTMap)
    : BaseTransferFunction(), m_bbToInstOUTMap(bbToInstOUTMap) {}

llvm::BitVector FaintTransferFunction::run(const llvm::BitVector &input,
                                           const llvm::BitVector &genSet,
                                           const llvm::BitVector &killSet,
                                           const llvm::BasicBlock &BB) {

        unsigned long bbSize = BB.size();

        // Run parent transfer function, this will be returned regardless:
        llvm::BitVector retVal =
            BaseTransferFunction::run(input, genSet, killSet, BB);
        DBUG(outs() << "FaintTransferFunction OUT: "
                    << "\n";
             BBInOutBits::printBitVector(retVal, MAX_PRINT_SIZE););

        // If vector's size == BB's num instructions, this is a second
        // iteration, clear the vector.
        std::vector<llvm::BitVector> *instOUTs =
            m_bbToInstOUTMap.find(&BB)->second;

        if (instOUTs->size() >= bbSize + 1) {
                instOUTs->clear();
        }

        // If vector is empty, insert to front, input set.
        if (instOUTs->empty()) {
                instOUTs->insert(instOUTs->begin(), input);
        }

        // If vector is less than BB's num instructions, we
        // aren't done with this bb yet, insert result of xfer function to it.
        if (instOUTs->size() < bbSize) {
                instOUTs->insert(instOUTs->begin(), retVal);
        }

        return retVal;
}

llvm::BitVector KillGenFaint::killEval(llvm::Instruction *I,
                                       llvm::BitVector &meet_res,
                                       std::vector<Value *> &domainset) {
        // constkill U depKill is the kill
        llvm::BitVector BBKill = constKillHelper(I, meet_res, domainset);
        BBKill |= depKillHelper(I, meet_res, domainset);
        return BBKill;
}

llvm::BitVector KillGenFaint::genEval(llvm::Instruction *I,
                                      llvm::BitVector &meet_res,
                                      std::vector<Value *> &domainset) {
        llvm::BitVector BBgen(MAX_BITS_SIZE);
        // We only need a constgen, so no need for helpers, calculate in here
        if (isa<BinaryOperator>(*I) || isa<PHINode>(*I) || isa<CastInst>(*I) ||
            isa<CmpInst>(*I)) {
                // If A is of the form x = e and x!= oper(e), gen it.
                // In SSA, we will never be in our own operand list,
                // single definition, so, just gen if you see
                setBitsIfInDomain(I, BBgen, domainset);
        }

        DBUG(outs() << "Meet Result is: \n";
             printDomainBits(meet_res, domainset);
             outs() << "-------------------------------------------\n";
             outs() << "Gen Result is: \n"; printDomainBits(BBgen, domainset);
             outs() << "-------------------------------------------\n";);

        return BBgen;
}

void KillGenFaint::printDomainBits(llvm::BitVector &bits,
                                   std::vector<Value *> &domainset) {
        for (int bitIndex : bits.set_bits()) {
                if (bitIndex < (int)domainset.size()) {
                        outs() << *domainset[bitIndex] << "\n";
                }
        }
}

void KillGenFaint::setBitsIfInDomain(const Value *V, llvm::BitVector &bits,
                                     std::vector<Value *> &domainset) {
        std::vector<Value *>::iterator it =
            std::find(domainset.begin(), domainset.end(), V);

        if (it != domainset.end()) {
                bits.set((size_t)(it - domainset.begin()));
        }
}

llvm::BitVector KillGenFaint::constKillHelper(llvm::Instruction *I,
                                              llvm::BitVector &meet_res,
                                              std::vector<Value *> &domainset) {
        // Iterate through instructions, if they are users of of a value in the
        // domainset AND they are non assignment statements, then kill. If they
        // are br instructions for instance (which are terminator instructions),
        // if they are debug info, or landingpad instructions
        llvm::BitVector constKillSet(MAX_BITS_SIZE);
        // TODO: Check if correct, we assume first operand for br,
        // indbr, switch, etc. is the target
        if (isa<TerminatorInst>(*I) || isa<LandingPadInst>(*I) ||
            isa<llvm::CmpInst>(*I) || I->mayHaveSideEffects()) {
                setBitsIfInDomain(I->getOperand(0), constKillSet, domainset);
        }
        // We won't generate faintness for any calls since we cannot guarantee
        // that they don't change global state, however, we can kill faintness
        // of any args going in
        if (isa<CallInst>(*I)) {
                for (auto OP = I->op_begin(); OP != I->op_end(); ++OP) {
                        setBitsIfInDomain(OP->get(), constKillSet, domainset);
                }
        }

        DBUG(outs() << "ConstKills are: \n";
             printDomainBits(constKillSet, domainset);
             outs() << "-------------------------------------------\n";);

        return constKillSet;
}

bool KillGenFaint::isValueInOUT(const Value *V, const llvm::BitVector &OUT,
                                const std::vector<Value *> &domainset) {
        bool retVal = false;
        for (int bitIndex : OUT.set_bits()) {
                if (bitIndex < (int)domainset.size()) {
                        if (V == domainset[bitIndex])
                                retVal = true;
                }
        }
        return retVal;
}

llvm::BitVector KillGenFaint::depKillHelper(llvm::Instruction *I,
                                            llvm::BitVector &meet_res,
                                            std::vector<Value *> &domainset) {

        llvm::BitVector depKillSet(MAX_BITS_SIZE);
        // If we are an assignment statement (either binary op or phi node) AND
        // x not in OUT. meet_res holds the bits that are entering, so they are
        // OUT.
        if (isa<BinaryOperator>(*I) || isa<PHINode>(*I) || isa<CastInst>(*I) ||
            isa<CmpInst>(*I)) {
                // If the LHS is not in OUT, then that e's
                // faintness was killed in the BB. Thus, add
                // operands of e as killed.
                if (!isValueInOUT(I, meet_res, domainset)) {
                        for (auto OP = I->op_begin(); OP != I->op_end(); ++OP) {
                                setBitsIfInDomain(OP->get(), depKillSet,
                                                  domainset);
                        }
                }
        }

        DBUG(outs() << "DepKills are: \n";
             printDomainBits(depKillSet, domainset);
             outs() << "-------------------------------------------\n";);

        return depKillSet;
}

FaintnessPass::FaintnessPass() : FunctionPass(ID) {}

bool FaintnessPass::runOnFunction(Function &F) {
        // Start of our modifications
        outs() << "Faintness for Function: " << F.getName() << "\n";

        // For now we only add binary op instructions, phi nodes, call inst,
        // invoke inst, and cast inst which includes trunc operations
        for (BasicBlock &BB : F) {
                for (Instruction &I : BB) {
                        std::pair<BasicBlock *, std::vector<BitVector> *>
                            mapvals;
                        mapvals.first = &BB;
                        mapvals.second = new std::vector<BitVector>();
                        m_bbToInstOUTMap.insert(mapvals);

                        if (isa<BinaryOperator>(I) || isa<PHINode>(I) ||
                            isa<CastInst>(I) || isa<CmpInst>(I)) {
                                m_domainSet.push_back(&I);
                        }
                }
        }

        outs() << "Domain is: \n";
        for (Value *v : m_domainSet) {
                outs() << *v << "\n";
        }
        outs() << "======================================================\n";

        // Instantiate requirements
        IntersectionMeet intersect;
        KillGenFaint killGenFaint;
        FaintTransferFunction transferFunc(m_bbToInstOUTMap);
        DataflowFramework<Value *> DF(intersect, BACKWARD, UNIVERSAL, F,
                                      m_domainSet, killGenFaint, transferFunc,
                                      INSTRUCTION);
        m_faintness = DF.run();

        // Did not modify the incoming Function.
        return false;
}

void FaintnessPass::getAnalysisUsage(AnalysisUsage &AU) const {
        AU.setPreservesAll();
}

llvm::DenseMap<BasicBlock *, std::vector<BitVector> *> *
FaintnessPass::getFaintResults() {
        return &m_bbToInstOUTMap;
}

std::vector<Value *> FaintnessPass::getDomainSet() { return m_domainSet; }

char FaintnessPass::ID = 0;
RegisterPass<FaintnessPass> X("faintness", "ECE 5984 Faint Analysis Pass");
