#include "types.h"

extern u8 D_801ACEE0[];
extern void func_80087824(void *buffer, s32 value, s32 size);

void func_800873D0(void) {
    func_80087824(D_801ACEE0, 0, 0x4000);
}
