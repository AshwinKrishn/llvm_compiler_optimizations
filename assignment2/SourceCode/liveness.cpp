// ECE 5984 S20 Assignment 2: liveness.cpp
// Group:

////////////////////////////////////////////////////////////////////////////////
#include "KillGen.h"
#include "MeetOpInterface.h"
#include "dataflow.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"

using namespace llvm;

namespace {

class KillGenLive : public KillGen {
      public:
	std::bitset<MAX_BITS_SIZE>
	KillEval(llvm::BasicBlock *BB,
		 std::map<std::string, unsigned int> list) {
		for (Instruction &I : *BB) {
		}
	}
	std::bitset<MAX_BITS_SIZE>
	GenEval(llvm::BasicBlock *BB,
		std::map<std::string, unsigned int> list) {
		for (Instruction &I : *BB) {
		}
	}
};
class Liveness : public FunctionPass, public KillGenLive {
      public:
	static char ID;

	Liveness() : FunctionPass(ID) {}

	virtual bool runOnFunction(Function &F) {

		// Did not modify the incoming Function.
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
