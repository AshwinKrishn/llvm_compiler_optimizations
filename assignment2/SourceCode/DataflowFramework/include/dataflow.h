
////////////////////////////////////////////////////////////////////////////////

#ifndef __CLASSICAL_DATAFLOW_H__
#define __CLASSICAL_DATAFLOW_H__

#include "llvm/ADT/BitVector.h"
#include "llvm/ADT/DenseMap.h"
#include "llvm/IR/CFG.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/ValueMap.h"

enum MeetOperator { UNION, INTERSECTION };
enum FlowDirection { FORWARD, BACKWARD };

namespace llvm {

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.

} // namespace llvm

#endif
