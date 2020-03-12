#ifndef __MEET_OP_INTERFACE__
#define __MEET_OP_INTERFACE__

#include <bitset>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <vector>

#define MAX_BITS_SIZE 4096
/*
 * Interface for meet operator
 *
 */
class IMeetOp {
    virtual std::bitset<MAX_BITS_SIZE>
    meet(std::vector<std::bitset<MAX_BITS_SIZE>> input) = 0;
};

#endif
