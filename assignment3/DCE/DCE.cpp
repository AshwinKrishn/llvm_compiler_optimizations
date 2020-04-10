// ECE 5984 S20 Assignment 3: dce.cpp
// Group: Sengming & Ashwin

////////////////////////////////////////////////////////////////////////////////
#include "KillGen.h"
#include "MeetOpInterface.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include <IntersectionMeet.h>
#include <KillGen.h>
#include <dataflow.h>
#include <vector>
using namespace llvm;

namespace {

/**
 * @brief Dead code elimintation function pass
 */
class DCE : public FunctionPass {
      public:
        static char ID;
        DCE() : FunctionPass(ID) {}

        virtual bool runOnFunction(Function &F) { return true; }

        virtual void getAnalysisUsage(AnalysisUsage &AU) const {
                AU.setPreservesAll();
        }

      private:
};

char DCE::ID = 0;
RegisterPass<DCE> X("dce", "ECE 5984 Dead Code Elimination");
} // namespace
