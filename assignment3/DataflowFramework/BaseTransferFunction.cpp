#include <BaseTransferFunction.h>
#include <llvm/ADT/BitVector.h>
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
llvm::BitVector BaseTransferFunction::run(const llvm::BitVector &input,
                                          const llvm::BitVector &genSet,
                                          const llvm::BitVector &killSet) {
        llvm::BitVector retval = genSet;
        // Make a copy of killset so we can flip the bits
        llvm::BitVector killSetCopy = killSet;
        llvm::BitVector killed = input;

        killed &= killSetCopy.flip();

        retval |= killed;
        return retval;
}

} // namespace llvm
