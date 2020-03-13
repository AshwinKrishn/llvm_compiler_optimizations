
////////////////////////////////////////////////////////////////////////////////
#include "dataflow.h"
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Support/raw_ostream.h>

namespace llvm {
// Add code for your dataflow abstraction here.

DataflowFramework::DataflowFramework(IMeetOp &meetOp, FlowDirection direction,
                                     Function &function)
    : m_meetOp(meetOp), m_func(function), m_dir(direction) {}

void DataflowFramework::run() {
    if (m_dir == FORWARD) {
        doForwardTraversal();
    } else {
        doBackwardTraversal();
    }
}

void DataflowFramework::doForwardTraversal() {
    // For now this will suffice, but we need to be wary of multiple return
    // statements, if that is present in the program. In LLVM there can be
    // multiple terminator instructions that are returns in a basic block. There
    // is no 'EXIT' block for the procedure. This means our ipo_begin
    // instantiation might not be 100% accurate. This also applies for
    // post_order.
    for (ipo_iterator<BasicBlock *> I =
             ipo_begin(&m_func.getBasicBlockList().back());
         I != ipo_end(&m_func.getEntryBlock()); ++I) {
        if (BasicBlock *BB = dyn_cast<BasicBlock>(*I))
            outs() << *BB << "\n";
    }
}

void DataflowFramework::doBackwardTraversal() {
    for (po_iterator<BasicBlock *> I = po_begin(&m_func.getEntryBlock());
         I != po_end(&m_func.getBasicBlockList().back()); ++I) {
        if (BasicBlock *BB = dyn_cast<BasicBlock>(*I))
            outs() << *BB << "\n";
    }
}

} // namespace llvm
