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
#include <llvm/Support/Errc.h>
#include <llvm/Support/Error.h>
#include <llvm/Support/raw_ostream.h>

// std
#include <vector>

// Local includes:
#include <BaseTransferFunction.h>
#include <KillGen.h>
#include <MeetOpInterface.h>
#include <available-support.h>

enum MeetOperator { UNION, INTERSECTION };
enum FlowDirection { FORWARD, BACKWARD };
enum BitsVal { ZEROS, ONES };
enum BoundaryCondition { EMPTY, UNIVERSAL };

namespace llvm {

class BBInOutBits {
      public:
        BBInOutBits(BitsVal inval, BitsVal outval) {
                if (inval == ZEROS)
                        m_IN.reset();
                else if (inval == ONES)
                        m_IN.set();

                if (outval == ZEROS)
                        m_OUT.reset();
                else if (outval == ONES)
                        m_OUT.set();
        }
        std::bitset<MAX_BITS_SIZE> m_IN;
        std::bitset<MAX_BITS_SIZE> m_OUT;
};

// Add definitions (and code, depending on your strategy) for your dataflow
// abstraction here.
template <typename D> class DataflowFramework {
      protected:
        IMeetOp &m_meetOp;
        Function &m_func;
        FlowDirection m_dir;
        BoundaryCondition m_boundary;
        std::vector<D> &m_domainSet;
        KillGen<D> &m_KG;
        BaseTransferFunction &m_transferFunc;

        void doForwardTraversal(
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentInOutMap,
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousInOutMap);
        void doBackwardTraversal(
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentInOutMap,
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousInOutMap);
        void initializeBbBitMaps(
            Function &F,
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap);
        bool
        hasOutChanged(llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap,
                      llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousMap);

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
                          BoundaryCondition boundary, Function &function,
                          std::vector<D> &domainset, KillGen<D> &KillGenImp,
                          BaseTransferFunction &transfer);
        // Placeholder function for entire computation, we may use template
        // later, depending on how we want to return the results and what
        // results to return. Eg: vector<Expression>? or vector<Variable>? We
        // might use vector<T> to template it.
        std::vector<D> &run();
};

template <typename D>
DataflowFramework<D>::DataflowFramework(
    IMeetOp &meetOp, FlowDirection direction, BoundaryCondition boundary,
    Function &function, std::vector<D> &domainset, KillGen<D> &KillGenImp)
    : m_meetOp(meetOp), m_func(function), m_dir(direction),
      m_boundary(boundary), m_domainSet(domainset), m_KG(KillGenImp) {}

template <typename D> std::vector<D> &DataflowFramework<D>::run() {
        llvm::DenseMap<BasicBlock *, BBInOutBits *> currentInOutMap;
        llvm::DenseMap<BasicBlock *, BBInOutBits *> previousInOutMap;

        // Initialize Maps
        initializeBbBitMaps(m_func, currentInOutMap);
        previousInOutMap = currentInOutMap;

        if (m_dir == FORWARD) {
                doForwardTraversal(currentInOutMap, previousInOutMap);
        } else {
                doBackwardTraversal(currentInOutMap, previousInOutMap);
        }
        return m_domainSet;
}

/**
 * @brief
 * Initializes the basic block bitmaps for in and out. In charge of creating the
 * bit vectors and associating them with the basic blocks.
 *
 * @tparam D Domain we operate on
 * @param F Function reference we're operating on
 * @param currentMap Map reference for basic block pointer to IN OUT bitmap
 * mapping
 */
template <typename D>
void DataflowFramework<D>::initializeBbBitMaps(
    Function &F, llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap) {
        BasicBlock &entry = F.getEntryBlock();
        BasicBlock &exit = F.getBasicBlockList().back();
        // Usually we prefer not to use pointers because owners
        // of the memory are not very obvious. In this case, we
        // have to use pointers because of the DenseMap. This is
        // ok as long as we don't delete entries from the map.
        // As for the pair itself, the DenseMap will copy the
        // pair in, so even if the pair goes out of scope, there
        // is no issue.
        if (m_dir == FORWARD) {
                for (BasicBlock &BB : F) {
                        BBInOutBits *p_inOut = new BBInOutBits(ZEROS, ONES);
                        std::pair<BasicBlock *, BBInOutBits *> p_pair;
                        p_pair.first = &BB;
                        p_pair.second = p_inOut;
                        currentMap.insert(p_pair);
                }
                // Explicitly clear or set the entry block's IN, while the
                // algorithm requires we clear/set an entry block's OUT, there
                // is no empty entry block, in the conventional sense in LLVM.
                // This is the first block, which is effectively the first block
                // after entry:
                if (m_boundary == UNIVERSAL) {
                        currentMap.find(&entry)->second->m_IN.set();
                } else if (m_boundary == EMPTY) {
                        currentMap.find(&entry)->second->m_IN.reset();
                }
        } else {
                for (BasicBlock &BB : F) {
                        BBInOutBits *p_inOut = new BBInOutBits(ONES, ZEROS);
                        std::pair<BasicBlock *, BBInOutBits *> p_pair;
                        p_pair.first = &BB;
                        p_pair.second = p_inOut;
                        currentMap.insert(p_pair);
                }
                // Explicitly set the exit block's OUT, again, this doesn't
                // cover multiple exit points and assumes a single return in the
                // function, we can add a sentinel "exit" node later on if
                // required for our analysis. Again, while the algorithm
                // requires we set the "exit" block's IN, we don't have an empty
                // "exit" block, the "exit" block currently is just the last
                // block of the function, containing the return instruction,
                // thus, we set the OUT of the "exit" block:
                if (m_boundary == UNIVERSAL) {
                        currentMap.find(&exit)->second->m_OUT.set();
                } else if (m_boundary == EMPTY) {
                        currentMap.find(&exit)->second->m_OUT.reset();
                }
        }
}

/**
 * @brief
 * Checks if any of the OUT's of any basic blocks has changed, if it has, return
 * true, else return false
 *
 * @tparam D Domain we operate on
 * @param currentMap Current bitmap reference
 * @param previousMap Previous bitmap reference from previous iteration
 *
 * @return
 */
template <typename D>
bool DataflowFramework<D>::hasOutChanged(
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap,
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousMap) {
        bool retval = false;
        // Iterate over both maps and check if OUT is the same
        for (auto it = currentMap.begin(); it != currentMap.end(); ++it) {
                auto currentOut = it->second->m_OUT;
                // Get previous mapping
                auto previous = previousMap.find(it->first);
                if (previous != previousMap.end()) {
                        auto previousOut = previous->second->m_OUT;
                        if (currentOut != previousOut) {
                                retval = true;
                        }
                } else {
                        // Error, BB entry in current not found in previous,
                        // should be impossible
                        llvm_unreachable(
                            "BB Entry in Current not found in Previous");
                }
        }
        return retval;
}

template <typename D>
void DataflowFramework<D>::doForwardTraversal(
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentInOutMap,
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousInOutMap) {
        // For now this will suffice, but we need to be wary of multiple return
        // statements, if that is present in the program. In LLVM there can be
        // multiple terminator instructions that are returns in a basic block.
        // There is no 'EXIT' block for the procedure. This means our ipo_begin
        // instantiation might not be 100% accurate. This also applies for
        // post_order.
        BasicBlock *BB;
        std::bitset<MAX_BITS_SIZE> meet_res;
        if (m_boundary == UNIVERSAL) {
                meet_res.reset();
        } else {
                meet_res.set();
        }
        do {
                previousInOutMap = currentInOutMap;
                for (ipo_iterator<BasicBlock *> I =
                         ipo_begin(&m_func.getBasicBlockList().back());
                     I != ipo_end(&m_func.getEntryBlock()); ++I) {
                        // Pre-define for convenience so we don't have to keep
                        // looking it up
                        BBInOutBits *currentInOutBits = currentInOutMap[BB];

                        if (BB = dyn_cast<BasicBlock>(*I))
                                outs() << *BB << "\n";
                        // MEET OF ALL PREDECESSORS
                        for (BasicBlock *Pred : predecessors(BB)) {
                                BBInOutBits *ip1 = currentInOutMap[Pred];
                                meet_res = m_meetOp.meet(ip1->m_OUT, meet_res);
                        }
                        currentInOutBits->m_OUT = m_KG.genEval(
                            BB, meet_res); // OUTPUT = BITS GENERATED in this
                        // basic block killset = killEval(); // OUTPUT =
                        // DEFINITIONS THAT GET KILLED BY BASIC BLOCK
                        // transferFunction(genset, killset, m_meetOp); general
                        // implementation, OUTPUT = CURRENT_BITVECTOR
                }
        } while (hasOutChanged(currentInOutMap, previousInOutMap));
}

template <typename D>
void DataflowFramework<D>::doBackwardTraversal(
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentInOutMap,
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousInOutMap) {
        for (po_iterator<BasicBlock *> I = po_begin(&m_func.getEntryBlock());
             I != po_end(&m_func.getBasicBlockList().back()); ++I) {
                if (BasicBlock *BB = dyn_cast<BasicBlock>(*I))
                        outs() << *BB << "\n";
        }
}

} // namespace llvm

#endif
