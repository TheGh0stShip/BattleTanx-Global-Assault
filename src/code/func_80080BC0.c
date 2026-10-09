#include "types.h"

extern void func_80081FD8(void *state);

void func_80080BC0(void *object) {
    register f32 zero __asm__("$f0") = 0.0f;

    *(s16 *)((u8 *)object + 0x130) = 0;
    *(s16 *)((u8 *)object + 0x132) = 0;
    *(f32 *)((u8 *)object + 0x12C) = zero;
    *(f32 *)((u8 *)object + 0x128) = zero;
    func_80081FD8((u8 *)object + 0x134);
    *(s32 *)((u8 *)object + 0x14C) = 0;
}
