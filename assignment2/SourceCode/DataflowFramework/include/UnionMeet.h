#ifndef __UNION_MEET__
#define __UNION_MEET__
#include <MeetOpInterface.h>
#include <bitset>

/*
 *union operator inheriting pure virtual MeetOp
 */
class UnionMeet : public MeetOp {
  public:
    std::bitset<MAX_BITS_SIZE> result;
    void union_op(std::bitset<MAX_BITS_SIZE> B);
    std::bitset<MAX_BITS_SIZE>
    meet(std::vector<std::bitset<MAX_BITS_SIZE>> input) override;
};

#endif
