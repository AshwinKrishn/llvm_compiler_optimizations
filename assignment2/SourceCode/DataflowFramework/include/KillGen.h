#include <MeetOpInterface.h>
#include <UnionMeet.h>

namespace llvm {

class KillGenEval {
      public:
	std::bitset<MAX_BITS_SIZE> KillEval(llvm::BasicBlock *BB,
					    std::Map<string, unsigned int>) = 0;
	std::bitset<MAX_BITS_SIZE> GenEval(llvm::BasicBlock *BB,
					   std::Map<string, unsigned int>) = 0;
}

} // namespace llvm
