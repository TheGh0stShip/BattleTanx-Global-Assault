#include "types.h"

extern s32 Steps_InitStep_Free(u16 value);

u16 func_8007E778(u8* object) {
    return *(u16*)(object + 0xF4);
}

s32 func_8007E784(u8* object) {
    return Steps_InitStep_Free(*(u16*)(object + 0xF4));
}
