#include <MeetOpInterface.h>
#include <UnionMeet.h>
void UnionMeet::union_op(std::bitset<MAX_BITS_SIZE> B) { result |= B; }

std::bitset<MAX_BITS_SIZE>
UnionMeet::meet(std::vector<std::bitset<MAX_BITS_SIZE>> input) {
    for (unsigned int i = 0; i < input.size(); i++) {
        union_op(input[i]);
    }
    return result;
}
