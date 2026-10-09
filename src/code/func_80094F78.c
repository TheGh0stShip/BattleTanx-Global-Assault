#include "types.h"

extern void func_80085DA8(void *object, s32 arg1, s32 arg2);

void func_80094F78(void *object, s32 arg1) {
    if (*(s32 *)((u8 *)object + 0x98) == 11) {
        *(u8 *)((u8 *)object + 0x16C) |= 0x10;
    } else {
        *(s32 *)((u8 *)object + 0x168) = 26;
        func_80085DA8(object, arg1, 0);
    }
}
