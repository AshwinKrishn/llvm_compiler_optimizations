#ifndef __FAINTNESS_H__
#define __FAINTNESS_H__
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <dataflow.h>
#include <llvm/ADT/DenseMap.h>
#include <vector>

using namespace llvm;

class FaintTransferFunction : public BaseTransferFunction {
      protected:
        DenseMap<BasicBlock *, std::vector<llvm::BitVector> *>
            &m_bbToInstOUTMap;

      public:
        FaintTransferFunction(
            DenseMap<BasicBlock *, std::vector<llvm::BitVector> *>
                &bbToInstOUTMap);
        // Generalized transfer function, add extra implementation if required
        virtual llvm::BitVector run(const llvm::BitVector &input,
                                    const llvm::BitVector &genSet,
                                    const llvm::BitVector &killSet,
                                    const llvm::BasicBlock &BB) override;
};

/**
 * @brief KillGen for Faintness
 */
class KillGenFaint : public KillGen<Value *> {
      protected:
        llvm::BitVector constKillHelper(llvm::Instruction *I,
                                        llvm::BitVector &meet_res,
                                        std::vector<Value *> &domainset);

        llvm::BitVector depKillHelper(llvm::Instruction *I,
                                      llvm::BitVector &meet_res,
                                      std::vector<Value *> &domainset);

        void setBitsIfInDomain(const Value *V, llvm::BitVector &bits,
                               std::vector<Value *> &domainset);

        void printDomainBits(llvm::BitVector &bits,
                             std::vector<Value *> &domainset);

        bool isValueInOUT(const Value *V, const llvm::BitVector &OUT,
                          const std::vector<Value *> &domainset);

      public:
        llvm::BitVector killEval(llvm::Instruction *I,
                                 llvm::BitVector &meet_res,
                                 std::vector<Value *> &domainset) override;
        llvm::BitVector genEval(llvm::Instruction *I, llvm::BitVector &meet_res,
                                std::vector<Value *> &domainset) override;
};

/**
 * @brief Primary function pass to run Faint Analysis pass.
 */
class FaintnessPass : public FunctionPass {
      protected:
        std::vector<Value *> m_domainSet;
        DenseMap<BasicBlock *, BBInOutBits *> *m_faintness;
        // Create a map of BasicBlock* to vector of BitVectors playing the role
        // of the individual OUTs for each instruction in a bb
        DenseMap<BasicBlock *, std::vector<BitVector> *> m_bbToInstOUTMap;

      public:
        static char ID;
        FaintnessPass();
        DenseMap<BasicBlock *, std::vector<BitVector> *> *getFaintResults();
        std::vector<Value *> getDomainSet();
        virtual bool runOnFunction(Function &F);

        virtual void getAnalysisUsage(AnalysisUsage &AU) const;
};

#endif
