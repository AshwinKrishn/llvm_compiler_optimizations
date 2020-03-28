// ECE 5984 S20 Assignment 2: liveness.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////
#include "KillGen.h"
#include "MeetOpInterface.h"
#include "dataflow.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <available-support.h>
#include <dataflow.h>
#include <vector>
using namespace llvm;

namespace {

class KillGenLive : public KillGen<Value *> {
      public:
        KillGenLive() : KillGen() {}
        std::bitset<MAX_BITS_SIZE>
        killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                 std::vector<Value *> &domainset) override {
                std::bitset<MAX_BITS_SIZE> BBkill;
                for (Instruction &I : *BB) {
                        if ((isa<BinaryOperator>(I)) || (isa<PHINode>(I))) {
                                Value *Ins = &I;
                                std::vector<Value *>::iterator it = std::find(
                                    domainset.begin(), domainset.end(), Ins);
                                BBkill.set(it - domainset.begin());
                        }
                }
                /*   outs() << "Kill for the block"
                          << BBkill.to_string().substr(
                                 MAX_BITS_SIZE - MAX_PRINT_SIZE, MAX_BITS_SIZE)
                          << "\n";
                          */
                return BBkill;
        }
        std::bitset<MAX_BITS_SIZE>
        genEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                std::vector<Value *> &domainset) override {
                std::bitset<MAX_BITS_SIZE> BBgen;
                for (Instruction &I : *BB) {
                        // For liveness when a variable is used we set the bits
                        for (int i = 0; i < I.getNumOperands(); i++) {
                                if ((isa<BinaryOperator>(I)) ||
                                    (isa<PHINode>(I))) {
                                        if (!isa<Constant>(I.getOperand(i))) {
                                                Value *Ins = I.getOperand(i);

                                                std::vector<Value *>::iterator
                                                    it = std::find(
                                                        domainset.begin(),
                                                        domainset.end(), Ins);
                                                BBgen.set((size_t)(
                                                    it - domainset.begin()));
                                        }
                                }
                        }
                }
                /*               outs() << "Gen for the block"
                                      << BBgen.to_string().substr(
                                             MAX_BITS_SIZE - MAX_PRINT_SIZE,
                   MAX_BITS_SIZE)
                                      << "\n";
                  */
                return BBgen;
        }
};

class Liveness : public FunctionPass, public KillGenLive {
      public:
        static char ID;
        std::vector<Value *> list;
        Liveness() : FunctionPass(ID) {}

        virtual bool runOnFunction(Function &F) {
                // Did not modify the incoming Function.
                for (Function::iterator FI = F.begin(), FE = F.end(); FI != FE;
                     ++FI) {
                        BasicBlock *block = &*FI;

                        for (BasicBlock::iterator i = block->begin(),
                                                  e = block->end();
                             i != e; ++i) {
                                Instruction *I = &*i;
                                if ((isa<BinaryOperator>(I)) ||
                                    (isa<PHINode>(I))) {
                                        // Only Insert if unique
                                        if (std::find(list.begin(), list.end(),
                                                      I) == list.end()) {
                                                list.push_back(I);
                                        }
                                        for (int i = 0; i < I->getNumOperands();
                                             i++) {
                                                if (!isa<Constant>(
                                                        I->getOperand(i))) {
                                                        // Only Insert if unique
                                                        if (std::find(
                                                                list.begin(),
                                                                list.end(),
                                                                I->getOperand(
                                                                    i)) ==
                                                            list.end()) {
                                                                list.push_back(
                                                                    I->getOperand(
                                                                        i));
                                                        }
                                                }
                                        }
                                }
                        }
                }
                outs() << "Variables used by this function with length :"
                       << list.size() << "\n";
                unsigned int index = 0;
                for (auto it : list) {
                        outs() << index << " : " << *it << "\n ";
                        ++index;
                }
                UnionMeet m_UnionMeet;
                KillGenLive KillGenL;
                BaseTransferFunction transferFunc;
                DataflowFramework<Value *> DF(m_UnionMeet, BACKWARD, EMPTY, F,
                                              list, KillGenL, transferFunc);
                DF.run();
                return false;
        }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesAll();
        }

      private:
};

char Liveness::ID = 0;
RegisterPass<Liveness> X("liveness", "ECE 5984 Liveness");
} // namespace
