/* RODATA_VRAM 0x80073820 */
#include "types.h"

extern u32 D_80117EB4;
extern u16 D_803A5980;
extern void* D_803A5984;
extern u16 D_8011DC54;
extern u16 D_8011DC56;
extern u16 D_803A5972;
extern void* D_8011DC50;
extern u8 D_8011BCBC[];
extern u8 D_8011B68C[];
extern u8 D_8011BCD8[];
extern u8 D_8011BD78[];
extern u8 D_8011BE38[];
extern u8 D_8011BEE8[];
extern u8 D_8011BF98[];
extern u8 D_8011C048[];
extern u8 D_8011C108[];
extern u8 D_8011C1B8[];
extern u8 D_8011B6A8[];
extern u8 D_8011B758[];
extern u8 D_8011B7F8[];
extern u8 D_8011B888[];
extern u8 D_8011B918[];
extern u8 D_8011B9A8[];
extern u8 D_8011BA48[];
extern u8 D_8011BAE8[];
extern s32 D_802195D8;
extern s32 D_802195C8;
extern void* func_800BD880(void* menu);
extern void func_800C2528(void* menu, void* option);
extern void func_800BEBA8(void* arg0, s32 arg1, s32 arg2);
extern void func_800E92F0(void);

inline void func_800C2924(void) {
    switch (D_80117EB4) {
    case 1: D_803A5984 = D_8011BCD8; D_803A5980 = 10; break;
    case 3: D_803A5984 = D_8011BE38; D_803A5980 = 11; break;
    case 4: D_803A5984 = D_8011BEE8; D_803A5980 = 11; break;
    case 5: D_803A5984 = D_8011BF98; D_803A5980 = 11; break;
    case 6: D_803A5984 = D_8011C048; D_803A5980 = 12; break;
    case 7: D_803A5984 = D_8011C108; D_803A5980 = 11; break;
    case 8: D_803A5984 = D_8011C1B8; D_803A5980 = 5; break;
    case 2:
    default: D_803A5984 = D_8011BD78; D_803A5980 = 12; break;
    }
    if (D_8011DC54 >= D_803A5980) D_8011DC54 = D_803A5980 - 1;
}

inline void func_800C2A38(void) {
    switch (D_80117EB4) {
    case 1: D_803A5984 = D_8011B6A8; D_803A5980 = 11; break;
    case 3: D_803A5984 = D_8011B7F8; D_803A5980 = 9; break;
    case 4: D_803A5984 = D_8011B888; D_803A5980 = 9; break;
    case 5: D_803A5984 = D_8011B918; D_803A5980 = 9; break;
    case 6: D_803A5984 = D_8011B9A8; D_803A5980 = 10; break;
    case 7: D_803A5984 = D_8011BA48; D_803A5980 = 10; break;
    case 8: D_803A5984 = D_8011BAE8; D_803A5980 = 4; break;
    case 2:
    default: D_803A5984 = D_8011B758; D_803A5980 = 10; break;
    }
    if (D_8011DC54 >= D_803A5980 - 1) D_8011DC54 = D_803A5980 - 2;
}

void func_800C2B48(void) {
    if (D_8011DC50 == D_8011BCBC) func_800C2924();
    else func_800C2A38();
}

s32 func_800C2D7C(s32 arg0) {
    switch (D_803A5972) {
    case 1:
        D_8011DC50 = D_8011BCBC;
        func_800C2924();
        D_8011DC54 = 1;
        func_800C2528(D_8011DC50, func_800BD880(D_8011DC50));
        D_8011DC56 = 0;
        func_800BEBA8(D_8011DC50, arg0, 2);
        return 0;
    case 2:
        D_8011DC50 = D_8011B68C;
        func_800C2A38();
        D_8011DC54 = D_803A5980 - 2;
        func_800C2528(D_8011DC50, func_800BD880(D_8011DC50));
        D_8011DC56 = 0;
        func_800BEBA8(D_8011DC50, arg0, 2);
        return 0;
    case 3:
    default:
        D_802195D8 = 1;
        if (D_80117EB4 == 1) {
            D_802195C8 = 7;
            func_800E92F0();
        } else {
            D_802195C8 = 7;
        }
        return 3;
    }
}
