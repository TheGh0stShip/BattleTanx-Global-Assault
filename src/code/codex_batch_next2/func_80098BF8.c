#include "types.h"

extern u8 D_80217010;
extern void *D_80219200;
extern u8 D_80216DC0;
extern u8 D_80216E00[];
extern s32 D_80216FA0[];
extern s32 D_80216FD0[];

extern s32 osRecvMesg(void *queue, void *message, s32 flags);
extern s32 osSendMesg(void *queue, void *message, s32 flags);
extern s32 osMotorInit(void *queue, void *pfs, s32 controller);
extern s32 osMotorStop(void *pfs);

void func_80098BF8(void) {
    s32 controller;
    s32 empty;
    s32 *state;
    s32 *slot;
    u8 *pfs;

    osRecvMesg(&D_80217010, &D_80219200, 1);
    controller = 0;
    empty = -1;
    pfs = D_80216E00;
    state = D_80216FD0;
    slot = D_80216FA0;
    while (controller < 4) {
        if (*slot != empty) {
            *state = 0;
            osMotorInit(&D_80216DC0, pfs, controller);
            osMotorStop(pfs);
        }
        pfs += 0x68;
        state++;
        controller++;
        slot++;
    }
    osSendMesg(&D_80217010, &D_80219200, 0);
}
