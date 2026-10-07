#include "types.h"

extern u16 D_803A5988;
extern u8 D_8011A5C8[];
extern u8 D_8011AB78[];
extern u8 D_8011AD38[];
extern u8 D_8011AEF8[];
extern u8 D_8011B0B8[];
extern void func_800BEBA8(void* object, s32 value, s32 mode);
extern void func_800BF130(void* object);

void func_800C1638(void) {
    if (D_803A5988 != 0) {
        func_800BEBA8(D_8011A5C8, 0, 0);
        func_800BF130(D_8011AB78);
        func_800BF130(D_8011AD38);
        func_800BF130(D_8011AEF8);
        func_800BF130(D_8011B0B8);
    }
}
