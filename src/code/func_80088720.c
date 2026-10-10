#include "types.h"

extern s32 D_8021945C;

void func_80088720(void *state) {
    f32 initial = 64000.0f;
    s32 timer = D_8021945C;
    u8 flags = *((u8 *)state + 0x16C);

    *(s32 *)((u8 *)state + 0xB4) = 0;
    *(s32 *)((u8 *)state + 0xB8) = 0;
    *((u8 *)state + 0xC4) = 0;
    *((u8 *)state + 0xC5) = 0;
    *(f32 *)((u8 *)state + 0xBC) = initial;
    *(s32 *)((u8 *)state + 0xC0) = timer + 0x12C;
    *((u8 *)state + 0x16C) = flags & 0xFE;
}
