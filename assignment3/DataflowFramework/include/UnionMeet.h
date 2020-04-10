#ifndef __UNION_MEET__
#define __UNION_MEET__
#include <MeetOpInterface.h>

/**
 * @brief Union meet, implements the meet operation, and is able to set and get
 * the top element.
 */
class UnionMeet : public IMeetOp {
      public:
        UnionMeet();
        void setTopElem(BitsVal val) override;
        BitsVal getTopElem() override;
        llvm::BitVector union_op(llvm::BitVector ip1, llvm::BitVector ip2);
        llvm::BitVector meet(llvm::BitVector input1,
                             llvm::BitVector input2) override;
};

#endif
