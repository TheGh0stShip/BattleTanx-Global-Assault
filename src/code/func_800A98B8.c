#include "types.h"
#include "m2c_macros.h"

#ifndef NULL
#define NULL ((void *)0)
#endif

s32 func_800A98B8(void *arg0, void *arg1) {
    s32 value;
    u32 index;
    void *entry;

    if ((arg1 == NULL) || !(M2C_FIELD(arg1, s32 *, 0x1E0) & 1)) {
        value = M2C_FIELD(arg0, s32 *, 0x1B8);
        index = 0;
    } else {
        index = M2C_FIELD(arg1, u32 *, 0x9C);
        value = M2C_FIELD(arg1, s32 *, 4);
    }
    if ((value == 0) & (index < 3U)) {
        __asm__("addu %0,%1,%2" : "=r"(entry) : "r"(index * 4), "r"(arg0));
        do {
            entry += 4;
            value = M2C_FIELD(entry, s32 *, 0x1B8);
            index += 1;
        } while ((value == 0) & (index < 3U));
    }
    return value;
}
