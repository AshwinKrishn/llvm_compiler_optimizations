
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"

// Local includes:
#include <MeetOpInterface.h>

enum MeetOperator { UNION, INTERSECTION };
enum FlowDirection { FORWARD, BACKWARD };

namespace llvm {

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.
class DataflowFramework {
  protected:
    IMeetOp &m_meetOp;
    Function &m_func;
    FlowDirection m_dir;

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
                      Function &function);

    // Placeholder function for entire computation, we may use template later,
    // depending on how we want to return the results and what results to
    // return. Eg: vector<Expression>? or vector<Variable>? We might use
    // vector<T> to template it.
    void run();
};

} // namespace llvm

#endif
