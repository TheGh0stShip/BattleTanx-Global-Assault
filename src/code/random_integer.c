#include "types.h"

extern u32 D_80114C80;

u32 func_8009D914(void) {
    u32 value;

    D_80114C80 = D_80114C80 * 69069 + 1;
    value = D_80114C80;
    return (value << 16) | (value >> 16);
}
