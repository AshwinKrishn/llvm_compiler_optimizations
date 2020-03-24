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

class KillGen {
      public:
	virtual std::bitset<MAX_BITS_SIZE>
	//	KillEval(llvm::BasicBlock *BB, std::vector<llvm::Expression>
	// list) { clean up
	KillEval(llvm::BasicBlock *BB, void *list) {
		outs() << "Needs overriding. Do not use default implementation";
	}
	virtual std::bitset<MAX_BITS_SIZE> GenEval(llvm::BasicBlock *BB,
						   void *list) {
		outs() << "Needs overriding Do not use default implementation";
	}
};

} // namespace llvm
#endif
