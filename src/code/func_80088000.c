#include "types.h"

extern s32 D_8021945C;

void func_80088000(void *state) {
    f32 initial = 64000.0f;
    s32 timer = D_8021945C;

    *(s32 *)state = 0;
    *(s32 *)((u8 *)state + 4) = 0;
    *((u8 *)state + 0x10) = 0;
    *((u8 *)state + 0x11) = 0;
    *(f32 *)((u8 *)state + 8) = initial;
    *(s32 *)((u8 *)state + 0xC) = timer + 0x12C;
}
