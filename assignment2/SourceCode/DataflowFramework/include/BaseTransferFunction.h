#ifndef __BASE_TRANSFER_FUNCTION_H__
#define __BASE_TRANSFER_FUNCTION_H__
#include <MeetOpInterface.h>
#include <bitset>
#include <vector>

namespace llvm {
/**
 * @brief Holds the base implementation of a transfer function, to be extended
 * later if we require additional steps to be added to the transfer function
 */
class BaseTransferFunction {
      public:
        // Generalized transfer function, add extra implementation if required
        virtual std::bitset<MAX_BITS_SIZE>
        run(const std::bitset<MAX_BITS_SIZE> &input,
            const std::bitset<MAX_BITS_SIZE> &genSet,
            const std::bitset<MAX_BITS_SIZE> &killSet);
};

} // namespace llvm
#endif
