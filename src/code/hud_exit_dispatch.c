/* RODATA_VRAM 0x80073668 */
#include "types.h"

extern u16 D_803A5970;
extern s32 D_80117EB4;
extern u16 D_80117F48;
extern s32 D_80117F44;
extern u16 D_80121CC0;
extern u8 D_80119C20;
extern s16 D_8011DC58;
extern s16 D_8011DC5A;
extern u8 D_80121CE0;
extern s32 D_802195D8;
extern s32 D_802195C8;
extern void* D_8011DC50;
extern u8 D_8011931C[];
extern u8 D_8011A3D4[];
extern u8 D_8011D9CC[];
extern u8 D_B0581C00[];
extern u8 D_B058FAD1[];
extern void func_800BEBA8(void* arg0, s32 arg1, s32 arg2);
extern void func_800CD730(s32 arg0, s32 arg1, s32 arg2);
extern void func_800D520C(void* start, void* end);
extern void func_800C2B48(void);
extern void func_800C27EC(void);
extern void func_800E92F0(void);

s32 func_800C08E0(s32 arg0) {
    switch (D_803A5970) {
    case 1:
        func_800BEBA8(D_8011931C, arg0, 1);
        break;
    case 2:
        D_80121CC0 = 0;
        D_80119C20 = 0;
        D_8011DC58 = 0;
        func_800BEBA8(D_8011A3D4, arg0, 1);
        break;
    case 3:
        func_800CD730(5, 0, 2);
        break;
    case 5:
        D_80121CE0 = 2;
        func_800D520C(D_B0581C00, D_B058FAD1);
        break;
    case 4:
    default:
        if (D_80117EB4 == 1) {
            if (D_80117F48 != 0) {
                func_800C2B48();
                func_800C27EC();
                func_800BEBA8(D_8011DC50, arg0, 1);
            } else {
                D_802195D8 = 1;
                D_802195C8 = 7;
                func_800E92F0();
            }
        } else if (D_80117EB4 == 6) {
            D_80117F44 = 17;
            D_802195D8 = 1;
            D_802195C8 = 7;
        } else {
            func_800BEBA8(D_8011D9CC, arg0, 1);
        }
        break;
    }
    D_8011DC5A = 0;
    return 1;
}
