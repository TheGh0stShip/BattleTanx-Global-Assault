#include "types.h"

extern u8 *D_8021945C;
extern void func_80088720(void *object);
extern void func_80088FD4(void *object);

void func_80082D10(void *object) {
    register s32 flags __asm__("$2") = *(s32 *)((u8 *)object + 0x1E0);
    register u8 *target __asm__("$5") = D_8021945C;

    flags &= ~0x100;
    target += 0x78;
    *(s32 *)((u8 *)object + 0x1E0) = flags;
    *(void **)((u8 *)object + 0x174) = target;
    func_80088720(object);
    func_80088FD4(object);
}
