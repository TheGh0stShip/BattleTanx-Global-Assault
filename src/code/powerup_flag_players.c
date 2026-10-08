/* ---- 0x800E4800/r2c/func_800E7968.c ---- */
#include "types.h"
#define NULL 0

typedef struct Item { u8 pad0[0xA]; u8 unkA; u8 pad0B[0x245]; } Item;
typedef struct Part { u8 pad0[0x36]; u8 unk36; } Part;

extern Item D_80235F00[];
extern u8 D_802194A6[];

static inline Item* getItem(s32 slot) {
    if (slot == 0x7F) return NULL;
    return &D_80235F00[slot];
}

void func_800E7968(Part* p) {
    s32 i;
    Item* it;

    if (p->unk36 != 0x7F) {
        getItem(p->unk36)->unkA |= 4;
    } else {
        for (i = 0; i < D_802194A6[0]; i++) {
            it = getItem(i);
            if (!(it->unkA & 2)) {
                it->unkA |= 4;
            }
        }
    }
}

