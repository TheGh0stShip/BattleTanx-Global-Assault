/* RODATA_VRAM 0x80073290 */
#include "types.h"

typedef struct {
    u16 flags;
    u8 pad2[0xE];
} HudNode;

extern HudNode D_803A57F0[];
extern s32 D_802195D4;
extern s32 D_802195C4;
extern u32 D_802195CC;
extern u16 D_80116840;
extern s16 D_803A5972;
extern s16 D_803A5970;
extern void* D_803A5954;
extern void* D_803A5958;
extern void* D_803A5960;
extern void* D_803A5964;
extern u64 D_803A5938;
extern s64 D_80126E50;
extern f32 D_803A5948;
extern f32 D_80219488;
extern u64 osGetTime(void);
extern void func_800A1384(void);
extern void func_800ACE70(void);
extern void func_800A9D50(void);
extern void* func_800ACEB4(s32 size);
extern void func_800CB110(void);
extern void func_800BFEA0(void);
extern void func_800CFDD0(void);
extern void func_800CD970(void);
extern void func_80098B40(void);
extern void func_80098B2C(void);
extern void func_800BF204(void);
extern void func_800BF484(void);

void func_800BF80C(void) {
    u64 now;
    u16 i;

    if (D_802195D4 == 0 || D_802195C4 == 0) {
        func_800A1384();
        D_803A5938 = osGetTime();
        for (i = 0; i < 20; i++) {
            D_803A57F0[i].flags = 0;
        }
        D_80116840 = 0;
        D_803A5972 = 0;
        D_803A5970 = 0;
        func_800ACE70();
        func_800A9D50();
        D_803A5954 = func_800ACEB4(60000);
        D_803A5958 = func_800ACEB4(60000);
        D_803A5960 = func_800ACEB4(0x780);
        D_803A5964 = func_800ACEB4(0x780);
        switch (D_802195CC) {
        case 1:
            func_800CB110();
            break;
        case 2:
            func_800BFEA0();
            break;
        case 8:
            func_800CFDD0();
            break;
        case 0:
        default:
            func_800CD970();
            break;
        }
    }
    func_80098B40();
    func_800BF204();
    func_80098B2C();
    func_800BF484();
    now = osGetTime();
    D_803A5948 = (f32)(((now - D_803A5938) * 1000000) / D_80126E50) * 30.0f / 1000000.0f;
    D_803A5938 = now;
}

void func_800BFAEC(void) {
    u64 now;

    func_800BF204();
    func_800BF484();
    switch (D_802195CC) {
    case 5:
    case 6:
    case 7:
        now = osGetTime();
        D_803A5948 = (f32)(((now - D_803A5938) * 1000000) / D_80126E50) * 30.0f / 1000000.0f;
        D_803A5938 = now;
        break;
    default:
        D_803A5948 = D_80219488;
        break;
    }
}
