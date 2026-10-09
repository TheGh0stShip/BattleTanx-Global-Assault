#include "types.h"

extern u8 D_801B4590;
extern u64 D_801B45B0;
extern u64 D_80126E50;
extern s32 D_801147E8;
extern u32 D_801B45A8;

extern s32 osSendMesg(void *queue, void *message, s32 flags);
extern u64 osGetTime(void);

void func_800976AC(void) {
    u64 elapsed;

    osSendMesg(&D_801B4590, 0, 1);
    D_801147E8 = 0;
    elapsed = osGetTime() - D_801B45B0;
    D_801B45A8 = (elapsed * 1000000U) / D_80126E50;
}
