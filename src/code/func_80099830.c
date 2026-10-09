#include "types.h"

extern void func_80097D14(void *value, s32 mode);

void func_80099830(void *object) {
    func_80097D14(*(void **)((u8 *)object + 0x6C), 10);
}
