#include "types.h"

extern u16 D_8011DC58;
extern u8 D_80119C20;
extern void func_80097530(void* state);

s32 func_800C1420(void) {
    if (D_8011DC58 != 0) {
        func_80097530(&D_80119C20);
        D_8011DC58--;
    }
    return 0;
}
