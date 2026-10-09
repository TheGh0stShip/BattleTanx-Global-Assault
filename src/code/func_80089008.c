#include "types.h"

extern f32 D_800716F4;

void func_80089008(void *state) {
    f32 initial = D_800716F4;

    *((u8 *)state + 0xE8) = 0;
    *(s32 *)((u8 *)state + 0xD4) = 0;
    *(s32 *)((u8 *)state + 0xD0) = 0;
    *(s32 *)((u8 *)state + 0xD8) = 0;
    *(s32 *)((u8 *)state + 0xDC) = 0;
    *(u16 *)((u8 *)state + 0xE4) = 0;
    *((u8 *)state + 0xE6) = 0;
    *((u8 *)state + 0xE7) = 0;
    *((u8 *)state + 0xE9) = 1;
    *(f32 *)((u8 *)state + 0xE0) = initial;
}
