#include "types.h"

extern u8 D_801194BC[];
extern void func_800BEBA8(void* object, s32 value, s32 mode);

void func_800C1578(s32 value) {
    func_800BEBA8(D_801194BC, value, 0);
}
