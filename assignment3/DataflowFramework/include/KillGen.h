#ifndef __KILLGEN__
#define __KILLGEN__
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include <MeetOpInterface.h>
#include <UnionMeet.h>
#include <map>
#include <string>
namespace llvm {

/**
 * @brief Interface class for the Kill and Gen functionality. Any new data flow
 * framework must implement their own killEval and genEval functions as they
 * each operate on their domain in their own ways.
 *
 * @tparam D Domain we operate on (variables/values or expressions)
 */
template <typename D> class KillGen {
      public:
        virtual std::bitset<MAX_BITS_SIZE>
        //	KillEval(llvm::BasicBlock *BB, std::vector<llvm::Expression>
        // list) { clean up
        killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                 std::vector<D> &domainset) = 0;
        virtual std::bitset<MAX_BITS_SIZE>
        genEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &meet_res,
                std::vector<D> &domainset) = 0;
};

} // namespace llvm
#endif
