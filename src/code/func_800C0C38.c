#include "types.h"

extern f32 D_803A5948;

u8 *func_8009836C(u16 id);

s32 func_800C0C38(u8 *object) {
    u8 *info = func_8009836C(*(u16 *)(object + 0x14));
    u8 type = info[0];
    register f32 value asm("$f0");
    register f32 factor asm("$f2");
    u8 *owner;
    u32 flags;

    if ((u8)(type + 10) >= 21) {
        factor = D_803A5948;
        value = (f32)((s8)type >> 4);
        value = value * factor;
        asm volatile("");
    } else {
        owner = *(u8 **)(object + 0xC);
        flags = *(u32 *)(info + 4);
        if (flags & *(u32 *)(owner + 0x18)) {
            value = D_803A5948 + D_803A5948;
        } else if (flags & *(u32 *)(owner + 0x14)) {
            value = D_803A5948;
            factor = -2.0f;
            value = value * factor;
        } else {
            return 0;
        }
    }
    return (s16)value;
}
