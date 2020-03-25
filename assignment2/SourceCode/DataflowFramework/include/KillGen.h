#ifndef __KILLGEN__
#define __KILLGEN__
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include <MeetOpInterface.h>
#include <UnionMeet.h>
#include <available-support.h>
#include <map>
#include <string>
namespace llvm {

template <typename D> class KillGen {
  public:
    virtual std::bitset<MAX_BITS_SIZE>
    //	KillEval(llvm::BasicBlock *BB, std::vector<llvm::Expression>
    // list) { clean up
    killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &list,
             std::bitset<MAX_BITS_SIZE> &depset, std::vector<D> &domainset) = 0;
    virtual std::bitset<MAX_BITS_SIZE>
    genEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &list,
            std::bitset<MAX_BITS_SIZE> &depset, std::vector<D> &domainset) = 0;
};

} // namespace llvm
#endif
