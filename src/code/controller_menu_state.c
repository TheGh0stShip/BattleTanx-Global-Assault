#include "types.h"

extern u8 D_8011FA40[];
extern s32 D_803A6358;
extern u16 D_8011F238;
extern s16 D_8011F232;
extern s16 D_8011F230;
extern s16 D_8011F236;
extern u8 D_8011F23A;
extern u8 D_8011F23B;
extern s32 D_803A62A8[];
extern s32 D_803A6354;
extern s32 D_803A62A0;
extern s16 D_803A6560;
extern s32 D_8021949C;
extern s32 D_802195D8;
extern s32 D_802195DC;
extern s32 D_802195CC;
extern s32 D_802195C8;
extern s16 func_800CC2F8(u16 count);
extern s16 func_800CCEC4(s32 arg0);

static inline void menu_queue_push(s32 cmd) {
    if (D_8011F230 < 10) {
        D_803A62A8[++D_8011F230] = cmd;
    }
}

void func_800CD57C(void* menu) {
    if (menu == D_8011FA40 && D_803A6358 != 4) {
        if (D_8011F238 != 0) {
            D_8011F238--;
        }
        D_8011F232 = func_800CC2F8(D_8011F238);
        if (D_8011F232 != 0) {
            menu_queue_push(6);
        }
    }
}

void func_800CD628(void* menu) {
    if (menu == D_8011FA40 && D_803A6358 != 4) {
        if (D_8011F238 < 15) {
            D_8011F238++;
        }
        D_8011F232 = func_800CC2F8(D_8011F238);
        if (D_8011F232 != 0) {
            menu_queue_push(6);
        }
    }
}

s32 func_800CD6D8(void) {
    if (D_803A6354 == 2) {
        D_802195D8 = 1;
        return 1;
    }
    D_802195D8 = D_803A6354;
    D_802195DC = 2;
    D_802195CC = 9;
    D_802195C8 = D_803A62A0;
    return 1;
}

void func_800CD730(s32 mode, s32 arg1, s32 arg2) {
    D_8011F23B = 0;
    D_803A6560 = 0;
    if (mode != 1 && mode != 2 && mode != 3 && mode != 5) {
        return;
    }
    if (D_8021949C == 16 && mode == 2) {
        return;
    }
    D_803A62A8[0] = mode;
    D_8011F230 = 0;
    D_8011F236 = 0;
    D_8011F23A = 1;
    D_803A6354 = arg1;
    D_803A62A0 = arg2;
    if (func_800CCEC4(0) != 0) {
        D_8011F23A = 0;
        D_802195D8 = 0;
        D_802195C8 = 1;
        return;
    }
    if (D_803A6354 == 2) {
        D_802195D8 = 1;
    } else {
        D_802195D8 = D_803A6354;
        D_802195DC = 2;
        D_802195CC = 9;
        D_802195C8 = D_803A62A0;
    }
}
