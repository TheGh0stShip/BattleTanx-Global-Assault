#include "types.h"

extern s32 D_8021D544;
extern u8 *D_80222420;
extern u32 D_8022291C;

s32 func_800A10E0(void) {
    u32 temp_v0;
    u8 *temp_v1;
    u8 temp_v2;

    temp_v0 = D_8022291C >> 1;
    D_8022291C = temp_v0;
    if (temp_v0 == 0) {
        temp_v1 = D_80222420;
        D_80222420 = temp_v1 + 1;
        temp_v2 = *temp_v1;
        D_8022291C = 0x80;
        D_8021D544 = temp_v2;
    }
    return (D_8021D544 & D_8022291C) != 0;
}
