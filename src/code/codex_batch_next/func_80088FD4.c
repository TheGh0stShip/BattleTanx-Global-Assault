#include "types.h"

extern f32 D_800716F0;

void func_80088FD4(void *state) {
    f32 initial = D_800716F0;

    *((u8 *)state + 0xE8) = 0;
    *(s32 *)((u8 *)state + 0xD4) = 0;
    *(s32 *)((u8 *)state + 0xD0) = 0;
    *(s32 *)((u8 *)state + 0xD8) = 0;
    *(s32 *)((u8 *)state + 0xDC) = 0;
    *(u16 *)((u8 *)state + 0xE4) = 0;
    *((u8 *)state + 0xE6) = 0;
    *((u8 *)state + 0xE7) = 0;
    *((u8 *)state + 0xE9) = 0;
    *(f32 *)((u8 *)state + 0xE0) = initial;
}
