#include "types.h"

extern u64 osGetTime(void);
extern u64 D_80219250;
extern s32 D_8021945C;
extern u64 D_80219478;
extern u64 D_80219480;
extern f32 D_80219488;
extern s32 D_8021948C;

void func_80099F74(void) {
    u64 now = osGetTime();

    D_80219478 = now;
    D_80219250 = now;
    D_80219480 = now;
    D_8021945C = 0;
    D_8021948C = 0x14;
    D_80219488 = 1.0f;
}
