#ifndef __KILLGEN__
#define __KILLGEN__
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include "llvm/Support/raw_ostream.h"
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
        virtual llvm::BitVector killEval(llvm::BasicBlock *BB,
                                         llvm::BitVector &meet_res,
                                         std::vector<D> &domainset) {
                llvm::BitVector bits(MAX_BITS_SIZE);
                outs() << "Base version of basic block level killEval used, "
                          "doesn't do anything.\n";
                return bits;
        }
        virtual llvm::BitVector genEval(llvm::BasicBlock *BB,
                                        llvm::BitVector &meet_res,
                                        std::vector<D> &domainset) {
                llvm::BitVector bits(MAX_BITS_SIZE);
                outs() << "Base version of basic block level genEval used, "
                          "doesn't do anything. \n";
                return bits;
        }
        virtual llvm::BitVector killEval(llvm::Instruction *I,
                                         llvm::BitVector &meet_res,
                                         std::vector<D> &domainset) {
                llvm::BitVector bits(MAX_BITS_SIZE);
                outs() << "Base version of instruction level killEval used, "
                          "doesn't do anything. \n";
                return bits;
        }
        virtual llvm::BitVector genEval(llvm::Instruction *I,
                                        llvm::BitVector &meet_res,
                                        std::vector<D> &domainset) {
                llvm::BitVector bits(MAX_BITS_SIZE);
                outs() << "Base version of instruction genEval used, doesn't "
                          "do anything. \n";

                return bits;
        }
};

} // namespace llvm
#endif
