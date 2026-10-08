#include "types.h"

extern void func_80088B6C(void *, void *, f32, s32);

/* Advance and apply the object's cyclic animation frame. */
void func_80082F1C(void *arg0) {
    register u8 *base __asm__("$7") = (u8 *)arg0 + 0x170;
    if (*(u8 *)((u8 *)arg0 + 0x16C) & 1) {
        register s32 frame __asm__("$5");
        frame = ((s32)*(u16 *)((u8 *)arg0 + 0x174) + 1) %
                (s32)*(u16 *)((u8 *)arg0 + 0x176);
        *(u16 *)((u8 *)arg0 + 0x174) = frame;
        func_80088B6C(arg0, base + ((frame * 8) + 8), 100.0f, 1);
    }
}
