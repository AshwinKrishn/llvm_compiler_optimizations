#ifndef __INTERSECTION_MEET__
#define __INTERSECTION_MEET__
#include <MeetOpInterface.h>
#include <bitset>

/*
 *intersection operator inheriting pure virtual MeetOp
 */
class IntersectionMeet : public MeetOp {

    std::bitset<MAX_BITS_SIZE> result;
    void intersection_op(std::bitset<MAX_BITS_SIZE> B);
    std::bitset<MAX_BITS_SIZE>
    meet(std::vector<std::bitset<MAX_BITS_SIZE>> input) override;
};

#endif
