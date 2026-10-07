#include "types.h"

extern void func_80088720(void* object);
extern void func_80088FD4(void* object);

void func_80082FE0(void) {
}

void func_80082FE8(u8* object) {
    *(u32*)(object + 0x1E0) &= ~0x100;
    func_80088720(object);
    func_80088FD4(object);
}
