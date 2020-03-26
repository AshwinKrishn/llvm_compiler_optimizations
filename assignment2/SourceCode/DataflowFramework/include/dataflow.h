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
enum BitsVal { ZEROS, ONES };

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
        std::vector<D> &m_domainSet;
        KillGen<D> &m_KG;
        void doForwardTraversal(
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentInOutMap,
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousInOutMap);
        void doBackwardTraversal(
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentInOutMap,
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousInOutMap);
        void initializeBbBitMaps(
            Function &F,
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap);

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
 * @param F Function reference
 * @param currentMap reference of map
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
                // Explicitly clear the entry block's OUT:
                currentMap.find(&entry)->second->m_OUT.reset();
        } else {
                for (BasicBlock &BB : F) {
                        BBInOutBits *p_inOut = new BBInOutBits(ONES, ZEROS);
                        std::pair<BasicBlock *, BBInOutBits *> p_pair;
                        p_pair.first = &BB;
                        p_pair.second = p_inOut;
                        currentMap.insert(p_pair);
                }
                // Explicitly set the exit block's IN:
                currentMap.find(&exit)->second->m_IN.set();
        }
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
        // Placeholders, remove later:
        // std::bitset<MAX_BITS_SIZE> domainbits;
        // std::bitset<MAX_BITS_SIZE> depkillset;

        // while (NO_CHANGES_TO_OUT)
        for (ipo_iterator<BasicBlock *> I =
                 ipo_begin(&m_func.getBasicBlockList().back());
             I != ipo_end(&m_func.getEntryBlock()); ++I) {
                if (BB = dyn_cast<BasicBlock>(*I))
                        outs() << *BB << "\n";
                // MEET OF ALL PREDECESSORS
                for (BasicBlock *Pred : predecessors(BB)) {
                        //	struct temp =  previous.find(BB);
                        //	meet_res = m_meetOp.meet(temp.output);
                        ////meet_res will be used for input in transffer
                        // function
                }
                // genset = m_KG.genEval(BB, domainbits, depkillset,
                // m_domainSet); // OUTPUT = BITS GENERATED in this basic block
                // killset = killEval(); // OUTPUT = DEFINITIONS THAT GET KILLED
                // BY BASIC BLOCK transferFunction(genset, killset, m_meetOp);
                // general implementation, OUTPUT = CURRENT_BITVECTOR
        }
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
