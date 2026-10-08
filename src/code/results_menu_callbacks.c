#include "types.h"

typedef struct {
    s32 pad[3];
    s32 flag;
    s32 pad10[3];
} DmaReq;

extern void func_80097D14(s32 arg0, s32 arg1);
extern void func_80096810(void* start, void* end, DmaReq* req, s32 slot);
extern void func_800BEBA8(void* arg0, s32 arg1, s32 arg2);
extern void func_800CD730(s32 arg0, s32 arg1, s32 arg2);
extern void func_800D520C(void* start, void* end);
extern void func_80098CC8(void);

extern u8 D_B04729C0[];
extern u8 D_B0472BAC[];
extern u8 D_B0472BB0[];
extern u8 D_B0472D9C[];
extern u8 D_B0472DA0[];
extern u8 D_B0472F8C[];
extern u8 D_B0514720[];
extern u8 D_B0529426[];
extern DmaReq D_80116860;
extern DmaReq D_8011687C;
extern DmaReq D_80116898;
extern s32 D_801168F8;
extern s32 D_80116914;
extern s32 D_80000300;
extern s32 D_80114804;
extern u8 D_8012008C[];
extern u8 D_8011FC30[];
extern u8 D_8011FB40[];
extern u8 D_8011FE9C[];
extern u8 D_8011FFB4[];
extern u16 D_803A6570;
extern s16 D_803A5970;
extern u8 D_80121CE0;
extern s32 D_802195D4;

void func_800CD970(void) {
    void* menu;

    func_80097D14(5, 0);
    D_80116860.flag = 0;
    D_8011687C.flag = 0;
    func_80096810(D_B04729C0, D_B0472BAC, &D_80116860, 0);
    func_80096810(D_B0472BB0, D_B0472D9C, &D_8011687C, 1);
    D_801168F8 = 0;
    D_80116914 = 0;
    if (D_80000300 == 0) {
        menu = D_8012008C;
    } else if (D_80114804 > 0) {
        menu = D_8011FC30;
    } else {
        D_80116898.flag = 0;
        func_80096810(D_B0472DA0, D_B0472F8C, &D_80116898, 2);
        menu = D_8011FB40;
    }
    func_800BEBA8(menu, 0, 1);
}

s32 func_800CDA78(void* arg0) {
    if (arg0 != D_8011FE9C || D_803A6570 != 0) {
        D_803A5970 = 1;
    }
    return 0;
}

s32 func_800CDAAC(void) {
    D_803A5970 = 1;
    return 0;
}

s32 func_800CDAC0(s32 arg0) {
    D_803A6570 = 0;
    func_800BEBA8(D_8011FE9C, arg0, 1);
    return 0;
}

s32 func_800CDAF4(s32 arg0) {
    func_800BEBA8(D_8011FFB4, arg0, 1);
    return 0;
}

s32 func_800CDB20(void) {
    func_800CD730(1, 0, 2);
    return 0;
}

s32 func_800CDB48(void) {
    D_80121CE0 = 1;
    func_800D520C(D_B0514720, D_B0529426);
    return 1;
}

s32 func_800CDB84(void) {
    D_803A6570 = 1;
    return 1;
}

s32 func_800CDB98(void) {
    func_80098CC8();
    if (D_80114804 > 0) {
        D_802195D4 = -1;
        return 1;
    }
    return 0;
}
