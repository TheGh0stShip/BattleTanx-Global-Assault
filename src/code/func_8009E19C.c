#include "types.h"

u64 func_8009E19C(u64 value, u8 *bit_indices) {
    u8 parity = 1;
    u32 low_bit;
    s32 index;

    for (index = 0; index < 6; index++) {
        if ((value >> bit_indices[index]) & 1) {
            parity ^= 1;
        }
    }
    value ^= parity;
    low_bit = value & 1;
    __asm__ volatile ("" : "=r" (low_bit) : "0" (low_bit));
    value >>= 1;
    return value | ((u64)low_bit << 63);
}
