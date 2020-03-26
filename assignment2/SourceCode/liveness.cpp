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

class KillGenLive : public KillGen<Value> {
      public:
	KillGenLive() : KillGen() {}
	std::bitset<MAX_BITS_SIZE> BBgen;
	std::bitset<MAX_BITS_SIZE>
	killEval(llvm::BasicBlock *BB, std::bitset<MAX_BITS_SIZE> &list,
		 std::bitset<MAX_BITS_SIZE> &depset,
		 std::vector<Value> &domainset) override {
		for (Instruction &I : *BB) {
			// null for Available expression
		}
		return list;
	}
	std::bitset<MAX_BITS_SIZE>
	genEval(llvm::BasicBlock *BB,
		std::bitset<MAX_BITS_SIZE> meet_res) override {
		for (Instruction &I : *BB) {

			//			set the bit for corresponding
			// expression;
			//			ret = in.find(Expression);
			//			list[ret] = 1;
		}
		return BBgen;
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
