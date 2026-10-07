#include "types.h"

extern u16 D_8011DC5A;
extern u16 D_803A5970;
extern void func_800979F4(s32 value);

s32 func_800C0850(void) {
    u16 active = D_8011DC5A;
    D_803A5970 = 4;
    if (active == 0) {
        func_800979F4(1);
        D_8011DC5A = 1;
    }
    return 0;
}

s32 func_800C0898(void) {
    u16 active = D_8011DC5A;
    D_803A5970 = 4;
    if (active == 0) {
        func_800979F4(1);
        D_8011DC5A = 1;
    }
    return 0;
}
