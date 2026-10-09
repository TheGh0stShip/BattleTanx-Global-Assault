#include "types.h"

extern void func_80088854(void *object);
extern void func_80089008(void *object);

void func_8008341C(void *object) {
    *(s32 *)((u8 *)object + 0x170) = 1;
    *(s32 *)((u8 *)object + 0x1E0) &= ~0x100;
    func_80088854(object);
    func_80089008(object);
}
