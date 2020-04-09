#ifndef __INTERSECTION_MEET__
#define __INTERSECTION_MEET__
#include <MeetOpInterface.h>
#include <llvm/ADT/BitVector.h>

/**
 * @brief Intersection meet, implements the meet operation, and is able to set
 * and get the top element.
 */
class IntersectionMeet : public IMeetOp {
      public:
        IntersectionMeet();
        void setTopElem(BitsVal val) override;
        BitsVal getTopElem() override;
        llvm::BitVector intersection_op(llvm::BitVector ip1,
                                        llvm::BitVector ip2);
        llvm::BitVector meet(llvm::BitVector input1,
                             llvm::BitVector input2) override;
};

#endif
