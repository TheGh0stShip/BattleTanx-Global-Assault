#include "types.h"

typedef struct {
    u8 pad0[0x14];
    s16 index;
    s16 y;
} HudSlot;

typedef struct {
    s32 key;
    u16 value;
} HudLookup;

extern s8 D_80117EB0;
extern u16 D_803A5988;
extern s32 D_80219208[];
extern HudSlot* D_8011DC60[];
extern HudLookup D_8011B124[];
extern void func_800BEBA8(HudSlot* slot, s32 arg1, s32 arg2);

s32 func_800C16B0(s32 arg0) {
    u16 i;
    s16 y;

    D_803A5988 = 0;
    i = 0;
    y = 0x9E - D_80117EB0 * 33;
    for (; i < D_80117EB0; i++) {
        HudSlot* slot = D_8011DC60[D_80219208[i]];

        slot->y = y;
        slot->index = i;
        func_800BEBA8(slot, arg0, 0);
        D_803A5988++;
        y += 0x42;
    }
    return 1;
}

u16 func_800C1790(s32 key) {
    HudLookup* p = D_8011B124;

    while (p->key != 0) {
        if (p->key == key) {
            break;
        }
        p++;
    }
    return p->value;
}
