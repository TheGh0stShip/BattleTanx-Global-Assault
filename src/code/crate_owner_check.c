#include "types.h"
#define NULL 0

typedef struct Part {
    u8 pad0[0xC]; s32 unk0C; s32 unk10; u8 pad14[0x4]; void* unk18; u8 pad1C[0x14];
    s32 unk30; u8 unk34; u8 unk35;
} Part;
typedef struct Stat { u8 pad0[8]; f32 unk8; } Stat;
typedef struct Item { u8 pad0[0x10]; Stat* unk10; u8 pad14[0x23C]; } Item;

extern void func_800A9B64(void*, s32);
extern void func_800E66A8(Part*, void*);
extern Item D_80235F00[];
extern s32 D_802194A0;
extern u8 D_802194A4;
extern f32 D_80219488;
extern s32 D_802194B0[];
extern s32 D_8021945C;

static inline Item* getItem(s32 slot) {
    if (slot == 0x7F) return NULL;
    return &D_80235F00[slot];
}

void func_800E68BC(Part* p, s32* out) {
    f32 old;
    f32 cur;
    s32 i;

    if (p->unk10 == 3) {
        *out = 1;
        return;
    }
    if (D_802194A0 == 3 && p->unk10 == 0) {
        old = getItem(p->unk35)->unk10->unk8;
        cur = getItem(p->unk35)->unk10->unk8 += D_80219488;
        if (D_802194B0[0] - cur < 300.0f && D_802194B0[0] - old >= 300.0f) {
            for (i = 0; i < D_802194A4; i++) {
                func_800A9B64(getItem(i), 3);
            }
        } else if (D_802194B0[0] - cur < 900.0f && D_802194B0[0] - old >= 900.0f) {
            for (i = 0; i < D_802194A4; i++) {
                func_800A9B64(getItem(i), 2);
            }
        }
    }
    if (p->unk0C == 0 && p->unk10 == 1 && D_8021945C - p->unk30 >= 121) {
        func_800E66A8(p, p->unk18);
    }
}
