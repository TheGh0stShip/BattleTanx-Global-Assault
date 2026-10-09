#include "types.h"

extern s32 osRecvMesg(void *queue, s32 *message, s32 flags);
extern void func_800A140C(void *queue);
extern void func_800A15F0(void *queue);
extern void func_800A16F8(void *queue);
extern void func_800A179C(void *queue);
extern void func_800A17DC(void *queue);

void func_800A1290(void *queue) {
    s32 message;

    for (;;) {
        osRecvMesg(queue, &message, 1);
        switch (message) {
            case 0x29A:
                func_800A140C(queue);
                break;
            case 0x29B:
                func_800A15F0(queue);
                break;
            case 0x29C:
                func_800A16F8(queue);
                break;
            case 0x29D:
                func_800A179C(queue);
                break;
            case 0x29E:
                func_800A17DC(queue);
                break;
        }
    }
}
