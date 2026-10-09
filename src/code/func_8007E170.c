#include "types.h"

extern u8 D_80122EE0[];
extern void func_8007E1FC(void *);

/* Reset an object's path-following state and restore its configured speed. */
void func_8007E170(void *arg0) {
    u8 *object = arg0;
    s32 type = *(s32 *)(object + 0x98);
    register f32 zero __asm__("$f0") = 0.0f;
    *(s32 *)(object + 0xF0) = 0;
    *(s16 *)(object + 0xF4) = 0;
    *(s16 *)(object + 0xF6) = 0;
    *(s16 *)(object + 0xF8) = 0;
    *(s16 *)(object + 0xFC) = 0;
    *(s16 *)(object + 0xFE) = 0;
    *(s16 *)(object + 0x100) = 0;
    *(s16 *)(object + 0x102) = 0;
    *(s32 *)(object + 0x10C) = 0;
    *(s32 *)(object + 0x120) = 0;
    *(f32 *)(object + 0x114) = zero;
    *(f32 *)(object + 0x110) = zero;
    *(f32 *)(object + 0x11C) = zero;
    *(f32 *)(object + 0x118) = zero;
    *(f32 *)(object + 0x124) =
        (f32)*(u16 *)(D_80122EE0 + (type * 0xD0));
    func_8007E1FC(object + 0xF0);
}
