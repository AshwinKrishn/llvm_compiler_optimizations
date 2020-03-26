#include <MeetOpInterface.h>
#include <UnionMeet.h>
std::bitset<MAX_BITS_SIZE> UnionMeet::union_op(std::bitset<MAX_BITS_SIZE> ip1,
					       std::bitset<MAX_BITS_SIZE> ip2) {
	return ip1 | ip2;
}

std::bitset<MAX_BITS_SIZE> UnionMeet::meet(std::bitset<MAX_BITS_SIZE> input1,
					   std::bitset<MAX_BITS_SIZE> input2) {

	return union_op(input1, input2);
}
