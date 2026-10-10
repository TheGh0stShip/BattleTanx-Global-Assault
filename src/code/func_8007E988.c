#include "types.h"

extern void Steps_InitStepPool(void *state);

void func_8007E988(void *object) {
    *(s16 *)((u8 *)object + 0x132) = 0;
    Steps_InitStepPool((u8 *)object + 0xF4);
    *(s16 *)((u8 *)object + 0xF6) = 0;
    *(s32 *)((u8 *)object + 0xF0) = 0;
    *(s16 *)((u8 *)object + 0xFA) = 0;
    *(s16 *)((u8 *)object + 0x104) = 0;
    *(s16 *)((u8 *)object + 0x106) = 0;
    *(s32 *)((u8 *)object + 0x108) = 0;
}
