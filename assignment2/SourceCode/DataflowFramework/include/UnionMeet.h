#ifndef __UNION_MEET__
#define __UNION_MEET__
#include <MeetOpInterface.h>
#include <bitset>

/*
 *union operator inheriting pure virtual MeetOp
 */
class UnionMeet : public IMeetOp {
      public:
	std::bitset<MAX_BITS_SIZE> result;
	std::bitset<MAX_BITS_SIZE> union_op(std::bitset<MAX_BITS_SIZE> ip1,
					    std::bitset<MAX_BITS_SIZE> ip2);
	std::bitset<MAX_BITS_SIZE>
	meet(std::bitset<MAX_BITS_SIZE> input1,
	     std::bitset<MAX_BITS_SIZE> input2) override;
};

#endif
