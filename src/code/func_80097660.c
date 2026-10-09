#include "types.h"

extern u8 D_80216D98[];
extern u64 D_801B45B0;
extern s32 D_801147E8;
extern void osSendMesg(void *, void *, s32);
extern u64 osGetTime(void);

s32 func_80097660(void) {
    osSendMesg(D_80216D98, 0, 1);
    D_801B45B0 = osGetTime();
    return D_801147E8;
}
