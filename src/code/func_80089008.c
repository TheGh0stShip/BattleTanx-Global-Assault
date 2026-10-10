#include "types.h"


void func_80089008(void *state) {
    f32 initial = 32768.0f;

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
