// ECE5984 S18 Assignment 2: available-support.cpp

////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__
// llvm based
#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"
#include <llvm/ADT/PostOrderIterator.h>
#include <llvm/Support/raw_ostream.h>

// std
#include <vector>

// Local includes:
#include <KillGen.h>
#include <MeetOpInterface.h>
#include <available-support.h>

enum MeetOperator { UNION, INTERSECTION };
enum FlowDirection { FORWARD, BACKWARD };

namespace llvm {

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.
template <typename D> class DataflowFramework {
  protected:
    IMeetOp &m_meetOp;
    Function &m_func;
    FlowDirection m_dir;
    std::vector<D> &m_domainSet;
    KillGen<D> &m_KG;
    void doForwardTraversal();
    void doBackwardTraversal();

  public:
    // Dataflow Framework needs these items:
    // 1) The basic block tree
    // 2) Direction of analysis
    // 3) Meet Operator
    // 4) Transfer Function
    // 5) IN or OUT Initialization
    // 6) Boundary Condition
    // 7) Sets of Expressions/Variables
    DataflowFramework(IMeetOp &meetOp, FlowDirection direction,
                      Function &function, std::vector<D> &domainset,
                      KillGen<D> &KillGenImp);
    // Placeholder function for entire computation, we may use template
    // later, depending on how we want to return the results and what
    // results to return. Eg: vector<Expression>? or vector<Variable>? We
    // might use vector<T> to template it.
    std::vector<D> &run();
};

template <typename D>
DataflowFramework<D>::DataflowFramework(IMeetOp &meetOp,
                                        FlowDirection direction,
                                        Function &function,
                                        std::vector<D> &domainset,
                                        KillGen<D> &KillGenImp)
    : m_meetOp(meetOp), m_func(function), m_dir(direction),
      m_domainSet(domainset), m_KG(KillGenImp) {}

template <typename D> std::vector<D> &DataflowFramework<D>::run() {
    if (m_dir == FORWARD) {
        doForwardTraversal();
    } else {
        doBackwardTraversal();
    }
    return m_domainSet;
}

template <typename D> void DataflowFramework<D>::doForwardTraversal() {
    // For now this will suffice, but we need to be wary of multiple return
    // statements, if that is present in the program. In LLVM there can be
    // multiple terminator instructions that are returns in a basic block.
    // There is no 'EXIT' block for the procedure. This means our ipo_begin
    // instantiation might not be 100% accurate. This also applies for
    // post_order.
    BasicBlock *BB;
    // Placeholders, remove later:
    std::bitset<MAX_BITS_SIZE> domainbits;
    std::bitset<MAX_BITS_SIZE> depkillset;

    for (ipo_iterator<BasicBlock *> I =
             ipo_begin(&m_func.getBasicBlockList().back());
         I != ipo_end(&m_func.getEntryBlock()); ++I) {
        if (BB = dyn_cast<BasicBlock>(*I))
            outs() << *BB << "\n";

        m_KG.genEval(BB, domainbits, depkillset, m_domainSet);
    }
}

template <typename D> void DataflowFramework<D>::doBackwardTraversal() {
    for (po_iterator<BasicBlock *> I = po_begin(&m_func.getEntryBlock());
         I != po_end(&m_func.getBasicBlockList().back()); ++I) {
        if (BasicBlock *BB = dyn_cast<BasicBlock>(*I))
            outs() << *BB << "\n";
    }
}

} // namespace llvm

#endif
