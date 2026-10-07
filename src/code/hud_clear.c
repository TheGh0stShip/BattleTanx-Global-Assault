#include "types.h"

extern u8 D_8011AB78[];
extern u8 D_8011AD38[];
extern u8 D_8011AEF8[];
extern u8 D_8011B0B8[];
extern void func_800BF1A4(void* object);

s32 func_800C24A4(void) {
    func_800BF1A4(D_8011AB78);
    func_800BF1A4(D_8011AD38);
    func_800BF1A4(D_8011AEF8);
    func_800BF1A4(D_8011B0B8);
    return 3;
}
