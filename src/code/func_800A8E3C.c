#include "types.h"

extern void func_800A6ABC(void *state, s32 value);
extern void func_800A8B38(void *object);

void func_800A8E3C(void *object) {
    if (*(u8 *)((u8 *)object + 0x110) == 0) {
        *(u8 *)((u8 *)object + 0x110) = 1;
        func_800A6ABC((u8 *)object + 0x78,
                      *(s32 *)((u8 *)object + 0x10C));
        func_800A8B38(object);
    }
}
