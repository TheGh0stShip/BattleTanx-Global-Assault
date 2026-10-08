#include "types.h"

extern u8 D_80217010;
extern void *D_80219200;
extern u8 D_80216E00[];

extern s32 osRecvMesg(void *queue, void *message, s32 flags);
extern s32 osSendMesg(void *queue, void *message, s32 flags);
extern s32 osPfsFreeBlocks(void *pfs, s32 *bytes);

s32 func_80099784(s32 controller, s32 *bytes) {
    s32 result;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    result = osPfsFreeBlocks(D_80216E00 + (controller - 1) * 0x68, bytes);
    osSendMesg(&D_80217010, &D_80219200, 0);
    return result;
}
