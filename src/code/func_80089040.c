#include "types.h"

extern f32 D_800716F8;

void func_80089040(void *state, void *source) {
    f32 initial = D_800716F8;

    *((u8 *)state + 0xE9) = 5;
    *((u8 *)state + 0xE8) = 0;
    *(s32 *)((u8 *)state + 0xD4) = 0;
    *(s32 *)((u8 *)state + 0xD0) = 0;
    *(s32 *)((u8 *)state + 0xD8) = 0;
    *(s32 *)((u8 *)state + 0xDC) = 0;
    *(u16 *)((u8 *)state + 0xE4) = 0;
    *((u8 *)state + 0xE6) = 0;
    *((u8 *)state + 0xE7) = 0;
    *(void **)((u8 *)state + 0xEC) = source;
    *((u8 *)state + 0xE8) = 1;
    *(f32 *)((u8 *)state + 0xE0) = initial;
    *(f32 *)((u8 *)state + 0xD0) = *(f32 *)((u8 *)source + 0x10);
    *(f32 *)((u8 *)state + 0xD4) = *(f32 *)((u8 *)source + 0x14);
}
