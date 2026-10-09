#include "types.h"
#include "m2c_macros.h"

extern u8 D_80122EE0[];
extern u8 D_80122EE2[];

u16 func_8007D69C(void *arg0) {
    u16 result;

    if (M2C_FIELD(arg0, u8 *, 0x16C) & 8) {
        result = *(u16 *)(D_80122EE2 + (M2C_FIELD(arg0, s32 *, 0x98) * 0xD0));
    } else {
        result = *(u16 *)(D_80122EE0 + (M2C_FIELD(arg0, s32 *, 0x98) * 0xD0));
    }
    return result;
}
