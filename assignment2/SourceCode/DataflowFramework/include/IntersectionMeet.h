#ifndef __INTERSECTION_MEET__
#define __INTERSECTION_MEET__
#include <MeetOpInterface.h>
#include <bitset>

/*
 *intersection operator inheriting pure virtual MeetOp
 */
class IntersectionMeet : public IMeetOp {
      public:
        IntersectionMeet();
        void setTopElem(BitsVal val) override;
        BitsVal getTopElem() override;
        std::bitset<MAX_BITS_SIZE>
        intersection_op(std::bitset<MAX_BITS_SIZE> ip1,
                        std::bitset<MAX_BITS_SIZE> ip2);
        std::bitset<MAX_BITS_SIZE>
        meet(std::bitset<MAX_BITS_SIZE> input1,
             std::bitset<MAX_BITS_SIZE> input2) override;
};

#endif
