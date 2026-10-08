#include "types.h"

extern u8 D_8021C0A0[];

void _bzero(void *address, s32 size);
void osInvalDCache(void *address, s32 size);
s32 osPiStartDma(void *io, s32 priority, s32 direction, u32 devAddr,
                 void *dramAddr, u32 size, void *queue);
s32 osRecvMesg(void *queue, void *message, s32 flags);

void func_8009ED00(u32 devAddr, void *dramAddr, s32 size) {
    u8 io[0x18];
    u32 message;

    if (size != 0) {
        _bzero(io, sizeof(io));
        _bzero(&message, sizeof(message));
        osInvalDCache(dramAddr, size);
        osPiStartDma(io, 0, 0, devAddr, dramAddr, size, D_8021C0A0);
        osRecvMesg(D_8021C0A0, 0, 1);
    }
}
