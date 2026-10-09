#include "types.h"

extern void func_80088720(void *object);
extern void func_80089008(void *object);

void func_80084224(void *object) {
    *(s32 *)((u8 *)object + 0x170) = 1;
    *(s32 *)((u8 *)object + 0x174) = 0;
    *(s32 *)((u8 *)object + 0x178) = 0;
    *(s32 *)((u8 *)object + 0x17C) = 0;
    *(s32 *)((u8 *)object + 0x1E0) &= ~0x100;
    func_80088720(object);
    func_80089008(object);
}
