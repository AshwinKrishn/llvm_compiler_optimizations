#include <IntersectionMeet.h>
#include <MeetOpInterface.h>
#include <llvm/ADT/BitVector.h>

IntersectionMeet::IntersectionMeet() { setTopElem(ONES); }

llvm::BitVector IntersectionMeet::intersection_op(llvm::BitVector ip1,
                                                  llvm::BitVector ip2) {
        llvm::BitVector retval = ip1;
        retval &= ip2;
        return retval;
}

llvm::BitVector IntersectionMeet::meet(llvm::BitVector input1,
                                       llvm::BitVector input2) {

        return intersection_op(input1, input2);
}

void IntersectionMeet::setTopElem(BitsVal val) { m_topElem = val; }

BitsVal IntersectionMeet::getTopElem() { return m_topElem; }
