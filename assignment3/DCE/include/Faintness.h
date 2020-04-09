#ifndef __FAINTNESS_H__
#define __FAINTNESS_H__
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <dataflow.h>

using namespace llvm;

/**
 * @brief KillGen for Faintness
 */
class KillGenFaint : public KillGen<Value *> {
      protected:
        llvm::BitVector constKillHelper(llvm::BasicBlock *BB,
                                        llvm::BitVector &meet_res,
                                        std::vector<Value *> &domainset);

        llvm::BitVector depKillHelper(llvm::BasicBlock *BB,
                                      llvm::BitVector &meet_res,
                                      std::vector<Value *> &domainset);

        void setBitsIfInDomain(Value *V, llvm::BitVector &bits,
                               std::vector<Value *> &domainset);

      public:
        llvm::BitVector killEval(llvm::BasicBlock *BB,
                                 llvm::BitVector &meet_res,
                                 std::vector<Value *> &domainset) override;
        llvm::BitVector genEval(llvm::BasicBlock *BB, llvm::BitVector &meet_res,
                                std::vector<Value *> &domainset) override;
};

/**
 * @brief Primary function pass to run Faint Analysis pass.
 */
class FaintnessPass : public FunctionPass {
      public:
        static char ID;
        FaintnessPass();
        virtual bool runOnFunction(Function &F);

        virtual void getAnalysisUsage(AnalysisUsage &AU) const;
};

#endif
