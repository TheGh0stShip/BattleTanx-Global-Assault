#include "types.h"

extern u32 __osGetSR(void);
extern void func_80078E40(u32);

u32 func_80077C40(void) {
    u32 status = __osGetSR();

    func_80078E40(status & ~1);
    return status & 1;
}

void func_80077C78(u32 status) {
    u32 current = __osGetSR();

    func_80078E40(current | status);
}

void func_80077CA8(u32* address, u32 value) {
    while (*(volatile u32*)0xA4600010 & 3) {
    }
    *address = value;
}

u32 func_80077CE0(volatile u32* address) {
    while (*(volatile u32*)0xA4600010 & 3) {
    }
    return *address;
}

extern volatile s32 D_80127E30;

void func_80077D1C(s32 count) {
    for (D_80127E30 = 0; D_80127E30 < count; D_80127E30++) {
    }
}

u8 func_80077D64(u32 address) {
    u32 shift = (~address & 3) << 3;

    return (func_80077CE0((volatile u32*)(address & ~3)) >> shift) & 0xFF;
}
