#include "types.h"

extern u8 D_801AD128[];

void *func_80089DBC(u32 mask) {
    u16 index;

    if (mask == 0) {
        return 0;
    }
    index = 0;
    while ((mask & 1) == 0) {
        mask >>= 1;
        index++;
    }
    return D_801AD128 + index * 1228;
}
