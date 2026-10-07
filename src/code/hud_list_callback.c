#include "types.h"

extern u8 D_8011B3E0[];
extern void func_800BEBA8(void* object, s32 value, s32 mode);

void func_800C24FC(s32 value) {
    func_800BEBA8(D_8011B3E0, value, 0);
}
