#include <BaseTransferFunction.h>
namespace llvm {
/**
 * @brief General base transfer function main method. Takes in 3 const set
 * references, gen, kill, and input(IN for forward, OUT for backward analysis)
 * General form is [Gen U (In - Kill)]. To mimic the (In - Kill) without doing a
 * borrow operation, we flip the kill set and perform bitwise AND. Truth table
 * is 0-0 = 0; 0-1 = 0; 1-0 = 1; 1-1 = 0. Next, Union operation is synonymous to
 * bitwise OR. Truth table is 0 U 0 = 0; 0 U 1 = 1; 1 U 0 = 1; 1 U 1 = 1.
 *
 * @param input Input into function, can be IN or OUT depending on direction
 * @param genSet Gen set
 * @param killSet Kill set
 *
 * @return Copy out the bitset.
 */
std::bitset<MAX_BITS_SIZE>
BaseTransferFunction::run(const std::bitset<MAX_BITS_SIZE> &input,
                          const std::bitset<MAX_BITS_SIZE> &genSet,
                          const std::bitset<MAX_BITS_SIZE> &killSet) {
        std::bitset<MAX_BITS_SIZE> retval;
        // Make a copy of killset so we can flip the bits
        std::bitset<MAX_BITS_SIZE> killSetCopy = killSet;
        std::bitset<MAX_BITS_SIZE> killed = input & killSetCopy.flip();

        retval = genSet | killed;
        return retval;
}

} // namespace llvm
