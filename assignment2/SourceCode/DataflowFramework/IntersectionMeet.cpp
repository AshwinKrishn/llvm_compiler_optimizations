#include <IntersectionMeet.h>
#include <MeetOpInterface.h>

IntersectionMeet::IntersectionMeet() { setTopElem(ONES); }

std::bitset<MAX_BITS_SIZE>
IntersectionMeet::intersection_op(std::bitset<MAX_BITS_SIZE> ip1,
                                  std::bitset<MAX_BITS_SIZE> ip2) {
        return (ip2 & ip1);
}

std::bitset<MAX_BITS_SIZE>
IntersectionMeet::meet(std::bitset<MAX_BITS_SIZE> input1,
                       std::bitset<MAX_BITS_SIZE> input2) {

        return intersection_op(input1, input2);
}

void IntersectionMeet::setTopElem(BitsVal val) { m_topElem = val; }

BitsVal IntersectionMeet::getTopElem() { return m_topElem; }
