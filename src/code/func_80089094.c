#include "types.h"


void func_80089094(void *state, void *source) {
    f32 initial = 32768.0f;

    *((u8 *)state + 0xE9) = 6;
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
    *(f32 *)((u8 *)state + 0xD0) = *(f32 *)((u8 *)source + 0xC);
    *(f32 *)((u8 *)state + 0xD4) = *(f32 *)((u8 *)source + 0x10);
}
