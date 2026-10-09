#include "types.h"

u64 func_8009E0E8(u64 value, u8 *bit_indices) {
    u8 parity = 1;
    u8 carry;
    s32 index;

    carry = value >> 63;
    value <<= 1;
    value |= carry;
    for (index = 0; index < 6; index++) {
        if ((value >> bit_indices[index]) & 1) {
            parity ^= 1;
        }
    }
    return value ^ parity;
}
