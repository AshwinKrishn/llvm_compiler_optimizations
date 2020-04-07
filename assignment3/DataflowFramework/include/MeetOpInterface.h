#ifndef __MEET_OP_INTERFACE__
#define __MEET_OP_INTERFACE__

#include <bitset>
#include <iostream>
#include <queue>
#include <stdio.h>
#include <vector>

#define MAX_BITS_SIZE 4096
#define MAX_PRINT_SIZE 32
/*
 * Interface for meet operator
 *
 */

enum BitsVal { ZEROS, ONES };

/**
 * @brief MeetOperator pure virtual class. Any new meet operator to be added
 * must inherit this class and implement their own meet function and
 * getters/setters for the top element.
 */
class IMeetOp {
      public:
        BitsVal m_topElem;
        virtual std::bitset<MAX_BITS_SIZE>
        meet(std::bitset<MAX_BITS_SIZE> input1,
             std::bitset<MAX_BITS_SIZE> input2) = 0;
        virtual void setTopElem(BitsVal val) = 0;
        virtual BitsVal getTopElem() = 0;
};

#endif
