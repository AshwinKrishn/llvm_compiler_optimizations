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
        std::bitset<MAX_BITS_SIZE>
        constKillHelper(llvm::BasicBlock *BB,
                        std::bitset<MAX_BITS_SIZE> &meet_res,
                        std::vector<Value *> &domainset);

        std::bitset<MAX_BITS_SIZE>
        depKillHelper(llvm::BasicBlock *BB,
                      std::bitset<MAX_BITS_SIZE> &meet_res,
                      std::vector<Value *> &domainset);

        void setBitsIfInDomain(Value *V, std::bitset<MAX_BITS_SIZE> &bits,
                               std::vector<Value *> &domainset);

      public:
        std::bitset<MAX_BITS_SIZE>
        killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                 std::vector<Value *> &domainset) override;
        std::bitset<MAX_BITS_SIZE>
        genEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
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
