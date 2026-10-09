#include "types.h"

extern void func_80088720(void *object);
extern void func_80088FD4(void *object);

void func_80083188(void *object, s32 state, s32 target) {
    *(s32 *)((u8 *)object + 0x170) = state;
    *(s32 *)((u8 *)object + 0x174) = target;
    *(s32 *)((u8 *)object + 0x1E0) &= ~0x100;
    func_80088720(object);
    func_80088FD4(object);
}
