#include <MeetOpInterface.h>
#include <UnionMeet.h>

UnionMeet::UnionMeet() { setTopElem(ZEROS); }
llvm::BitVector UnionMeet::union_op(llvm::BitVector ip1, llvm::BitVector ip2) {
        llvm::BitVector retval = ip1;
        retval |= ip2;
        return retval;
}

llvm::BitVector UnionMeet::meet(llvm::BitVector input1,
                                llvm::BitVector input2) {

        return union_op(input1, input2);
}

void UnionMeet::setTopElem(BitsVal val) { m_topElem = val; }

BitsVal UnionMeet::getTopElem() { return m_topElem; }
