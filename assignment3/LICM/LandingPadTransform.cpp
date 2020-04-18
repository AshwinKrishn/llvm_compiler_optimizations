// ECE 5984 S20 Assignment 3: LandingPadTransform.cpp
// Group: Ashwin and SengMing

////////////////////////////////////////////////////////////////////////////////

#include "llvm/ADT/SmallVector.h"
#include "llvm/IR/Function.h"
#include "llvm/Pass.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Transforms/Utils/BasicBlockUtils.h"
#include <llvm/ADT/SmallVector.h>
#include <llvm/Analysis/CFGPrinter.h>
#include <llvm/IR/Instruction.h>
#include <llvm/Transforms/Utils/SSAUpdater.h>

// ours
#include <IntersectionMeet.h>
#include <LandingPadTransform.h>
#include <dataflow.h>

// llvm specific
#include <llvm/Analysis/LoopPass.h>

namespace llvm {

LandingPadTransform::LandingPadTransform() : LoopPass(ID) {}

void LandingPadTransform::updatePhiNotInLoop(
    Loop &loop, SmallVector<Instruction *, 10> &previousPhiUsers, PHINode *from,
    PHINode *to) {
        // Update all users of phi not in loop set
        std::vector<BasicBlock *> &loopBlocks = loop.getBlocksVector();
        for (Instruction *I : previousPhiUsers) {
                BasicBlock *usersBlock = I->getParent();
                DBUG(outs() << "Users: " << *I << "\n";);
                if ((std::find(loopBlocks.begin(), loopBlocks.end(),
                               usersBlock) == loopBlocks.end())) {
                        // Replace with our new phi
                        if (I != to) {
                                I->replaceUsesOfWith(from, to);
                        }
                }
        }
}

void LandingPadTransform::unifyPhiAtExit(BasicBlock *newtest,
                                         BasicBlock *unifiedExit,
                                         BasicBlock *loopexit,
                                         BasicBlock *header,
                                         BasicBlock *lastBody, Loop *L) {
        DBUG(std::vector<BasicBlock *> &loopBlocks = L->getBlocksVector(););
        SmallVector<PHINode *, 10> insertedPhi;
        SmallVector<Instruction *, 10> previousPhiUsers;

        DBUG(for (BasicBlock *BB : loopBlocks) { outs() << *BB << "\n"; });

        for (Instruction &I : *header) {
                if (PHINode *phi = dyn_cast<PHINode>(&I)) {
                        // Save previous users, for some reason after adding the
                        // exitphi many users of our phi are invalidated

                        for (User *U : phi->users()) {
                                if (Instruction *UI =
                                        dyn_cast<Instruction>(U)) {
                                        previousPhiUsers.push_back(UI);
                                }
                        }

                        PHINode *exitphi = PHINode::Create(I.getType(), 0);
                        exitphi->addIncoming(phi, lastBody);
                        exitphi->addIncoming(phi->getIncomingValue(0), newtest);
                        loopexit->getInstList().push_front(exitphi);
                        insertedPhi.push_back(phi);

                        DBUG(outs() << "Phi Instruction: " << *phi << "\n";);

                        updatePhiNotInLoop(*L, previousPhiUsers, phi, exitphi);
                }
                DBUG(outs() << "----------------------\n";);
        }
}

BasicBlock *LandingPadTransform::removePhiDependencies(BasicBlock *newtest,
                                                       BasicBlock *header) {
        BasicBlock *backedgeBlock = nullptr;
        for (Instruction &I : *header) {
                if (PHINode *phi = dyn_cast<PHINode>(&I)) {
                        // All backedges should be same, we just need
                        // any one of them
                        backedgeBlock = phi->getIncomingBlock(1);
                        Value *incomingValue = phi->getIncomingValue(0);
                        // Hopefully this doesn't kill us later
                        for (Instruction &newtestI : *newtest) {
                                for (auto OP = newtestI.op_begin();
                                     OP != newtestI.op_end(); ++OP) {
                                        if (OP->get() == &I) {
                                                OP->set(incomingValue);
                                        }
                                }
                        }

                        // for (Use &phi_use : phi->uses()) {
                        //        Instruction *firstval =
                        //            dyn_cast<Instruction>(phi_use);
                        //        outs() << "Phi uses: \n";
                        //        outs() << *phi_use.get() << "\n";
                        //        if (firstval->getParent() == newtest)
                        //        {
                        //                phi_use.set(incomingValue);
                        //        }
                        //}
                }
        }
        return backedgeBlock;
}

bool LandingPadTransform::runOnLoop(Loop *L, LPPassManager &LPM) {
        BasicBlock *oldpreheader = L->getLoopPreheader();
        BasicBlock *header = L->getHeader();
        LoopInfo &LI = getAnalysis<LoopInfoWrapperPass>().getLoopInfo();
        static int counter = 0;
        // If eligible for transform, add landing pad
        if (oldpreheader) {
                BasicBlock *newpreheader = oldpreheader->splitBasicBlock(
                    oldpreheader->getTerminator(), ".newpreheader");
                if (Loop *parent = L->getParentLoop()) {
                        parent->addBasicBlockToLoop(newpreheader, LI);
                }
                //    1) Update Phi instructions in header, these
                //    instructions will only have two sources, the loop
                //    back edge and the edge from preheader. Replace
                //    references with operand 0. Save back edge BB and
                //    remove phi instruction. 2) Change header
                //    terminator's first target to header, before that
                //    store original target as after_header. Second
                //    target save as exit. 3) Remove preheader's
                //    terminator 4) Move all instructions to preheader
                //    5) Use saved back edge BB and replace terminator
                //    with cloned cmp instruction, with one edge going
                //    back to after_header and another going to exit.

                // Splice all instructions in previous header to this
                // new split block
                oldpreheader->getTerminator()->eraseFromParent();
                oldpreheader->getInstList().splice(
                    oldpreheader->end(), header->getInstList(),
                    header->getFirstNonPHI()->getIterator(), header->end());

                // New Test block is technically the old preheader now
                BasicBlock *newtest = oldpreheader;

                BasicBlock *lastBody = removePhiDependencies(newtest, header);

                BasicBlock *bodyBlock = cast<BasicBlock>(
                    cast<BranchInst>(newtest->getTerminator())->getOperand(2));
                cast<BranchInst>(newtest->getTerminator())
                    ->setOperand(2, newpreheader);

                // Now current header no longer has terminator, add one
                // for it.
                // BranchInst *headerTerminator =
                BranchInst::Create(bodyBlock, header);

                // Update last body block to check condition and point
                // to exit or header;
                lastBody->getTerminator()->eraseFromParent();
                BranchInst *branchClone =
                    cast<BranchInst>(newtest->getTerminator()->clone());
                Instruction *beforeBranch =
                    &*(--newtest->getTerminator()->getIterator());
                CmpInst *cmpClone = cast<CmpInst>((beforeBranch)->clone());

                // We also want any instructions this cmp instruction
                // (beforeBranch) might have relied on

                branchClone->setOperand(2, header);

                lastBody->getInstList().push_back(cmpClone);
                lastBody->getInstList().push_back(branchClone);

                // Create our unified loop exit block by splitting
                // current loop exit
                BasicBlock *loopexit = dyn_cast<BasicBlock>(
                    newtest->getTerminator()->getOperand(1));
                assert(loopexit);

                BasicBlock *unifiedExit = loopexit->splitBasicBlock(
                    loopexit->begin(), ".unifiedExit");
                // for (Instruction &I : *unifiedExit) {
                //        for (auto OP = I.op_begin(); OP != I.op_end(); ++OP) {
                //                if (Instruction *OI =
                //                        dyn_cast<Instruction>(OP)) {
                //                        outs() << "Op Inst: " << *OI << "\n";
                //                        outs() << OI << "\n";
                //                        for (User *U : OI->users()) {
                //                                outs() << "Operand users: \n";
                //                                outs() << *U << "\n";
                //                        }
                //                }
                //        }
                //}

                if (Loop *parent = L->getParentLoop()) {
                        parent->addBasicBlockToLoop(unifiedExit, LI);
                }

                unifyPhiAtExit(newtest, unifiedExit, loopexit, header, lastBody,
                               L);
        }
        counter++;
        // if (counter == 2)
        //         oldpreheader->getParent()->viewCFG();

        return true;
}

void LandingPadTransform::getAnalysisUsage(AnalysisUsage &AU) const {
        // AU.setPreservesAll();
        // AU.setPreservesCFG();
        AU.addRequired<LoopInfoWrapperPass>();
}

char LandingPadTransform::ID = 2;
RegisterPass<LandingPadTransform> lpadtrans("landing-pad",
                                            "ECE 5984 Landing Pad");
} // namespace llvm
