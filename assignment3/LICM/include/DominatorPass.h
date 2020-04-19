#ifndef __DOMINATORS_H__
#define __DOMINATORS_H__
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <dataflow.h>
#include <llvm/ADT/BitVector.h>
namespace llvm {
class KillGenDoms : public KillGen<BasicBlock *> {
      public:
        llvm::BitVector killEval(llvm::BasicBlock *BB,
                                 llvm::BitVector &meet_res,
                                 std::vector<BasicBlock *> &domainset) override;
        llvm::BitVector genEval(llvm::BasicBlock *BB, llvm::BitVector &meet_res,
                                std::vector<BasicBlock *> &domainset) override;
};
class DominatorsPass : public FunctionPass {
      public:
        static char ID;
        std::vector<BasicBlock *> bb_ids;
        DenseMap<BasicBlock *, BBInOutBits *> *DomTree;
        std::vector<BasicBlock *> getBBIds();
        DenseMap<BasicBlock *, BBInOutBits *> *getDomResults();
        DominatorsPass();
        virtual bool runOnFunction(Function &F);

        virtual void getAnalysisUsage(AnalysisUsage &AU) const;
};
} // namespace llvm
#endif
