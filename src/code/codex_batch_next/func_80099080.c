#include "types.h"

extern u8 D_80217010;
extern void *D_80219200;
extern u8 D_80216DC0;
extern u8 D_80216E00[];
extern s32 D_80114800;

extern s32 osRecvMesg(void *queue, void *message, s32 flags);
extern s32 osSendMesg(void *queue, void *message, s32 flags);
extern s32 osPfsInitPak(void *queue, void *pfs, s32 controller);
extern s32 osMotorInit(void *queue, void *pfs, s32 controller);

s32 func_80099080(s32 controller) {
    s32 result;
    void *pfs;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    controller--;
    pfs = D_80216E00 + controller * 0x68;
    result = osPfsInitPak(&D_80216DC0, pfs, controller);
    if (result == 10 && osMotorInit(&D_80216DC0, pfs, controller) == 0) {
        result = 1;
    }
    osSendMesg(&D_80217010, &D_80219200, 0);
    if (result == 2) {
        D_80114800 = result;
    }
    return result;
}
