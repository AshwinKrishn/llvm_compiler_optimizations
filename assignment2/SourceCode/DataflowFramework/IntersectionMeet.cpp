#include <IntersectionMeet.h>
#include <MeetOpInterface.h>

void IntersectionMeet::intersection_op(std::bitset<MAX_BITS_SIZE> B) {
    result &= B;
}

std::bitset<MAX_BITS_SIZE>
IntersectionMeet::meet(std::vector<std::bitset<MAX_BITS_SIZE>> input) {
    for (unsigned int i = 0; i < input.size(); i++) {
        intersection_op(input[i]);
    }
    return result;
}
