/* RODATA_VRAM 0x80073F6C */
#include "types.h"

extern s32 D_8011E95C;
extern s32 D_8011E964;
extern f32 D_8011E96C;
extern f32 D_8011E9B4;
extern s16 D_8011EAD0;
extern s16 D_8011EAE0;
extern void* D_8011EAC4;
extern void** D_8011EAD8;
extern void** D_8011EAE8;
extern u8 D_8011EACC;
extern u8 D_8011EADC;
extern u8 D_80117E30[];
extern u8 D_80117E3C[];
extern u8 D_8011E9A4[];
extern u8 D_8011EA04[];
extern s32 D_803A5948;
extern u8 D_8011EAF0[];
extern void func_800BEBA8(void* arg0, s32 arg1, s32 arg2);

void func_800CAA48(s32 arg0, s32 arg1, f32 arg2, u16 arg3) {
    D_8011E95C = arg0;
    D_8011E964 = arg1;
    D_8011E96C = arg2;
    D_8011E9B4 = arg2 - 10.0f;
    if (arg3 != 0) {
        D_8011EAD0 = 30;
        D_8011EAE0 = 45;
        D_8011EAC4 = D_80117E3C;
        *D_8011EAD8 = 0;
        *D_8011EAE8 = 0;
    } else {
        D_8011EAC4 = D_80117E30;
        *D_8011EAD8 = D_8011E9A4;
        D_8011EACC = 9;
        *D_8011EAE8 = D_8011EA04;
        D_8011EADC = 9;
    }
    D_803A5948 = 0;
    func_800BEBA8(D_8011EAF0, 0, 1);
}
