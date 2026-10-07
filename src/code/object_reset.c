#include "types.h"

extern void func_800A6ABC(void* object, s32 value);

void func_800A8B14(u8* object, s32 value) {
    object[0x110] = 0;
    func_800A6ABC(object + 0x78, value);
}
