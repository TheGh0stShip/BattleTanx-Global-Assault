/* RODATA_VRAM 0x80073288 */
#include "types.h"

typedef struct {
    u16 flags;
    u16 field2;
    s8 parent;
    s8 children[4];
    u8 pad9[3];
    void* object;
} HudNode;

typedef struct {
    u32 w0;
    u32 w1;
} GfxWords;

extern HudNode D_803A57F0[];
extern u16 D_80116840;
extern GfxWords* D_803A5954;
extern GfxWords* D_803A5958;
extern void* D_803A5960;
extern void* D_803A5964;
extern GfxWords* D_803A5944;
extern void* D_803A5930;
extern u64 D_803A5938;
extern s64 D_80126E50;
extern f32 D_803A5948;
extern void func_800BC9F4(void* object);
extern void func_800BF3EC(u16 index);
extern void func_8007AD1C(GfxWords* list);
extern u64 osGetTime(void);

void func_800BF484(void) {
    s16 i;
    u16 k;
    GfxWords* list;

    if (D_80116840 == 0) {
        D_803A5944 = D_803A5954;
        D_803A5930 = D_803A5960;
    } else {
        D_803A5944 = D_803A5958;
        D_803A5930 = D_803A5964;
    }
    for (i = 0; i < 20; i++) {
        if (D_803A57F0[i].flags != 0 && D_803A57F0[i].parent < 0) {
            if (D_803A57F0[(u16)i].flags & 1) {
                func_800BC9F4(D_803A57F0[(u16)i].object);
            }
            for (k = 0; k < 4; k++) {
                if (D_803A57F0[(u16)i].children[k] > 0) {
                    func_800BF3EC(D_803A57F0[(u16)i].children[k]);
                }
            }
        }
    }
    if (D_80116840 == 0) {
        list = D_803A5954;
    } else {
        list = D_803A5958;
    }
    D_803A5944->w0 = 0xDF000000;
    D_803A5944->w1 = 0;
    func_8007AD1C(list);
    D_80116840 = 1 - D_80116840;
}

void func_800BF618(void) {
    u64 now = osGetTime();

    D_803A5948 = (f32)(((now - D_803A5938) * 1000000) / D_80126E50) * 30.0f / 1000000.0f;
    D_803A5938 = now;
}
