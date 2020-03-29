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
#include <llvm/Support/Debug.h>
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

// Debug
//#define DEBUG_TYPE "dataflow_framework"

enum MeetOperator { UNION, INTERSECTION };
enum FlowDirection { FORWARD, BACKWARD };
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
        void
        initializeBbBitMaps(Function &F,
                            llvm::DenseMap<BasicBlock *, BBInOutBits *> &map);
        bool
        hasOutChanged(llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap,
                      llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousMap);

        bool
        hasInChanged(llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap,
                     llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousMap);

        void deepCopyDenseMaps(
            llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap,
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
    Function &function, std::vector<D> &domainset, KillGen<D> &KillGenImp,
    BaseTransferFunction &transfer)
    : m_meetOp(meetOp), m_func(function), m_dir(direction),
      m_boundary(boundary), m_domainSet(domainset), m_KG(KillGenImp),
      m_transferFunc(transfer) {}

template <typename D> std::vector<D> &DataflowFramework<D>::run() {
        llvm::DenseMap<BasicBlock *, BBInOutBits *> currentInOutMap;
        llvm::DenseMap<BasicBlock *, BBInOutBits *> previousInOutMap;

        // Initialize Maps
        initializeBbBitMaps(m_func, currentInOutMap);
        initializeBbBitMaps(m_func, previousInOutMap);

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
    Function &F, llvm::DenseMap<BasicBlock *, BBInOutBits *> &map) {
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
                        BBInOutBits *p_inOut =
                            new BBInOutBits(ZEROS, m_meetOp.getTopElem());
                        std::pair<BasicBlock *, BBInOutBits *> p_pair;
                        p_pair.first = &BB;
                        p_pair.second = p_inOut;
                        map.insert(p_pair);
                }
                // Explicitly clear or set the entry block's IN, while the
                // algorithm requires we clear/set an entry block's OUT, there
                // is no empty entry block, in the conventional sense in LLVM.
                // This is the first block, which is effectively the first block
                // after entry:
                if (m_boundary == UNIVERSAL) {
                        map.find(&entry)->second->m_IN.set();
                } else if (m_boundary == EMPTY) {
                        map.find(&entry)->second->m_IN.reset();
                }
        } else {
                for (BasicBlock &BB : F) {
                        BBInOutBits *p_inOut =
                            new BBInOutBits(m_meetOp.getTopElem(), ZEROS);
                        std::pair<BasicBlock *, BBInOutBits *> p_pair;
                        p_pair.first = &BB;
                        p_pair.second = p_inOut;
                        map.insert(p_pair);
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
                        map.find(&exit)->second->m_OUT.set();
                } else if (m_boundary == EMPTY) {
                        map.find(&exit)->second->m_OUT.reset();
                }
        }
}

template <typename D>
void DataflowFramework<D>::deepCopyDenseMaps(
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap,
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousMap) {
        for (auto it = currentMap.begin(); it != currentMap.end(); ++it) {
                // Get previous mapping
                auto previous = previousMap.find(it->first);
                if (previous != previousMap.end()) {
                        previous->second->m_OUT = it->second->m_OUT;
                        previous->second->m_IN = it->second->m_IN;
                } else {
                        // Error, BB entry in current not found in previous,
                        // should be impossible
                        llvm_unreachable(
                            "BB Entry in Current not found in Previous");
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
                        // outs() << "Current OUT: "
                        //       << currentOut.to_string().substr(MAX_BITS_SIZE
                        //       -
                        //                                        MAX_PRINT_SIZE)
                        //       << "Previous OUT: "
                        //       << previousOut.to_string().substr(MAX_BITS_SIZE
                        //       -
                        //                                         MAX_PRINT_SIZE)
                        //       << "\n";
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

/**
 * @brief
 * Checks if any of the IN's of any basic blocks has changed, if it has, return
 * true, else return false
 *
 * @tparam D Domain we operate on
 * @param currentMap Current bitmap reference
 * @param previousMap Previous bitmap reference from previous iteration
 *
 * @return
 */
template <typename D>
bool DataflowFramework<D>::hasInChanged(
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentMap,
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousMap) {
        bool retval = false;
        // Iterate over both maps and check if OUT is the same
        for (auto it = currentMap.begin(); it != currentMap.end(); ++it) {
                auto currentIn = it->second->m_IN;
                // Get previous mapping
                auto previous = previousMap.find(it->first);
                if (previous != previousMap.end()) {
                        auto previousIn = previous->second->m_IN;
                        // outs() << "Current OUT: "
                        //       << currentOut.to_string().substr(MAX_BITS_SIZE
                        //       -
                        //                                        MAX_PRINT_SIZE)
                        //       << "Previous OUT: "
                        //       << previousOut.to_string().substr(MAX_BITS_SIZE
                        //       -
                        //                                         MAX_PRINT_SIZE)
                        //       << "\n";
                        if (currentIn != previousIn) {
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
        std::bitset<MAX_BITS_SIZE> meet_res, BB_killset, BB_genset;
        int iteration_no = 1;
        do {
                outs() << "ITERATION " << iteration_no << "\n";
                outs() << "++++++++++++++++++++++++++++++++++++++++++++++++++++"
                          "++++++++++++++++++++++++++++++++"
                       << "\n";
                // Make values of previous == current
                deepCopyDenseMaps(currentInOutMap, previousInOutMap);

                for (ipo_iterator<BasicBlock *> I =
                         ipo_begin(&m_func.getBasicBlockList().back());
                     I != ipo_end(&m_func.getEntryBlock()); ++I) {

                        if (BB = dyn_cast<BasicBlock>(*I))
                                outs() << *BB << "\n";
                        // Pre-define for convenience so we don't have to keep
                        // looking it up
                        BBInOutBits *currentInOutBits = currentInOutMap[BB];
                        // Actually, normally meet of all predecessors vanilla
                        // would work, but we need to initialize one of them to
                        // our m_IN. Recall that we don't actually have an empty
                        // ENTRY block. Our ENTRY block in llvm is actually
                        // conceptually the block after the empty ENTRY block.
                        // That's why we initialized the IN of this block to the
                        // Universal or Empty set. Making the initial met_res
                        // equal to the IN of the current block solves this for
                        // entry block. Else, if we're any other block, we take
                        // any of the predecessors and make the meet_res equal
                        // to that.
                        if (BB == &m_func.getEntryBlock()) {
                                meet_res = currentInOutBits->m_IN;
                        } else {
                                auto pred = pred_begin(BB);
                                meet_res = currentInOutMap[*pred]->m_OUT;
                        }
                        // MEET OF ALL PREDECESSORS
                        for (BasicBlock *Pred : predecessors(BB)) {
                                BBInOutBits *ip1 = currentInOutMap[Pred];
                                meet_res = m_meetOp.meet(ip1->m_OUT, meet_res);
                        }

                        outs() << "Current IN bits: "
                               << meet_res.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";

                        // Create genset and killset
                        BB_genset = m_KG.genEval(BB, meet_res, m_domainSet);
                        outs() << "Current GEN Set: "
                               << BB_genset.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";

                        BB_killset = m_KG.killEval(BB, meet_res, m_domainSet);
                        outs() << "Current KILL Set: "
                               << BB_killset.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";

                        // Run transfer function on our sets, store the bits:
                        currentInOutBits->m_OUT =
                            m_transferFunc.run(meet_res, BB_genset, BB_killset);
                        outs() << "Current OUT bits: "
                               << currentInOutBits->m_OUT.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";
                        outs() << "======================================"
                                  "======================================"
                               << "\n";
                }
                ++iteration_no;
        } while (hasOutChanged(currentInOutMap, previousInOutMap));
}

template <typename D>
void DataflowFramework<D>::doBackwardTraversal(
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &currentInOutMap,
    llvm::DenseMap<BasicBlock *, BBInOutBits *> &previousInOutMap) {
        BasicBlock *BB;
        std::bitset<MAX_BITS_SIZE> meet_res, BB_killset, BB_genset;
        int iteration_no = 1;
        do {
                outs() << "ITERATION " << iteration_no << "\n";
                outs() << "++++++++++++++++++++++++++++++++++++++++++++++++++++"
                          "++++++++++++++++++++++++++++++++"
                       << "\n";
                // Make values of previous == current
                deepCopyDenseMaps(currentInOutMap, previousInOutMap);

                for (po_iterator<BasicBlock *> I =
                         po_begin(&m_func.getEntryBlock());
                     I != po_end(&m_func.back()); ++I) {

                        if (BB = dyn_cast<BasicBlock>(*I))
                                outs() << *BB << "\n";
                        // Pre-define for convenience so we don't have to keep
                        // looking it up
                        BBInOutBits *currentInOutBits = currentInOutMap[BB];
                        // Actually, normally meet of all successors vanilla
                        // would work, but we need to initialize one of them to
                        // our m_IN. Recall that we don't actually have an empty
                        // ENTRY block. Our ENTRY block in llvm is actually
                        // conceptually the block after the empty ENTRY block.
                        // That's why we initialized the IN of this block to the
                        // Universal or Empty set. Making the initial met_res
                        // equal to the IN of the current block solves this for
                        // entry block. Else, if we're any other block, we take
                        // any of the successors and make the meet_res equal
                        // to that.
                        if (BB == &m_func.back()) {
                                meet_res = currentInOutBits->m_OUT;
                        } else {
                                auto succ = succ_begin(BB);
                                meet_res = currentInOutMap[*succ]->m_IN;
                        }
                        // MEET OF ALL PREDECESSORS
                        for (BasicBlock *Succ : successors(BB)) {
                                BBInOutBits *ip1 = currentInOutMap[Succ];
                                meet_res = m_meetOp.meet(ip1->m_IN, meet_res);
                        }

                        outs() << "Current OUT bits: "
                               << meet_res.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";

                        // Create genset and killset
                        BB_genset = m_KG.genEval(BB, meet_res, m_domainSet);
                        outs() << "Current GEN Set: "
                               << BB_genset.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";

                        BB_killset = m_KG.killEval(BB, meet_res, m_domainSet);
                        outs() << "Current KILL Set: "
                               << BB_killset.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";

                        // Run transfer function on our sets, store the bits:
                        currentInOutBits->m_IN =
                            m_transferFunc.run(meet_res, BB_genset, BB_killset);
                        outs() << "Current IN bits: "
                               << currentInOutBits->m_IN.to_string().substr(
                                      MAX_BITS_SIZE - MAX_PRINT_SIZE,
                                      MAX_BITS_SIZE)
                               << "\n";
                        outs() << "======================================"
                                  "======================================"
                               << "\n";
                }
                ++iteration_no;
        } while (hasInChanged(currentInOutMap, previousInOutMap));
}

} // namespace llvm

#endif
