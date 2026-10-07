#include "types.h"

extern u8 D_80118AAC[];
extern u8 D_8011A898[];
extern void func_800BEBA8(void* object, s32 value, s32 mode);

void func_800C1138(s32 value) {
    func_800BEBA8(D_80118AAC, value, 0);
}

s32 func_800C1164(s32 value) {
    func_800BEBA8(D_8011A898, value, 1);
    return 1;
}
