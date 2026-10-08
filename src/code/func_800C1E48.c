#include "types.h"

extern u16 D_803A5988;
extern u8 D_8011A5C8[];
extern u8 D_8011AB78[];
extern u8 D_8011AD38[];
extern u8 D_8011AEF8[];
extern u8 D_8011B0B8[];

u8 *func_8009836C(u16 id);
void func_800BEBA8(void *arg, s32 value, s32 value2);
void func_800BF130(void *arg);

s32 func_800C1E48(u8 *object) {
    u32 flags = *(u32 *)(func_8009836C(*(u16 *)(object + 0x14)) + 8);

    if (flags & 0xD000) {
        D_803A5988--;
        return 1;
    }
    if (flags & 0x2000) {
        if (D_803A5988 != 0) {
            func_800BEBA8(D_8011A5C8, 0, 0);
            func_800BF130(D_8011AB78);
            func_800BF130(D_8011AD38);
            func_800BF130(D_8011AEF8);
            func_800BF130(D_8011B0B8);
        }
    }
    return 0;
}
