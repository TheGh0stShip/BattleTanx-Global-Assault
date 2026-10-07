#include "types.h"

extern s32 func_800F2810(void);
extern void* func_800E562C(void);
extern void* func_800A18D0(s32 arg0, s32 arg1);

s32 func_800A2DFC(void) {
    s32 result;

    if (func_800F2810()) {
        result = 1;
    } else {
        result = func_800E562C() != 0;
    }
    return result;
}

void func_800A2E30(void) {
    s32* entry = func_800A18D0(0, 0x10);

    if (entry != 0) {
        entry[3] = 0;
    }
}
