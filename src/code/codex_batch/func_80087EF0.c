#include "types.h"

extern s32 func_800890E8(void *, void *);

void func_80087EF0(void *arg0, void *arg1) {
    s32 result = func_800890E8(arg0, (u8 *)arg1 + 0xC);

    *(u16 *)((u8 *)arg1 + 0xA) = result;
    if (*(s32 *)((u8 *)arg1 + 0x10) != 0) {
        *(u16 *)((u8 *)arg1 + 8) = result;
    }
}
