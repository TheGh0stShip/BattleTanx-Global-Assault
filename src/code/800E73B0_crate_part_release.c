/* Unit 0x800E73B0..0x800E759C (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/r3/e4800/func_800E73B0_partial.c (7 words off) fixed in this lane.
 * Fix: case 14 loop written as `n = &D_80224EF0[D_80224E76]; while (1) { ... if (n->next == -1) break; n = &D_80224EF0[n->next]; }` (no idx variable), which puts the compare constants at the top of the preheader (see strategy/CAUSES.md R21).
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800E759C */
#include "types.h"
#define NULL 0

typedef struct Part {
    u8 pad0[0x10]; s32 unk10; u8 pad14[0x8]; s32 unk1C; struct Part* next;
    u8 pad24[0x11]; u8 unk35;
} Part;
typedef struct Item { u8 pad0[0x10]; s32 unk10; u8 pad14[0x23C]; } Item;
typedef struct Node { u8 pad0[0x8]; s16 next; u8 pad0A[6]; s32 unk10; u8 pad14[0x30]; } Node;
typedef struct Obj73B0 {
    s32 unk0; u8 pad4[0x9C]; Part* list; u8 padA4[0x12C]; Item* unk1D0;
} Obj73B0;

extern void func_800A9B64(void*, s32);
extern u32 D_802194A0;
extern s32 D_8021949C;
extern u8 D_802194A4;
extern Item D_80235F00[];
extern Node D_80224EF0[];
extern s16 D_80224E76;

static inline Item* getItem(s32 slot) {
    if (slot == 0x7F) return NULL;
    return &D_80235F00[slot];
}

void func_800E73B0(Obj73B0* arg0, Part* p) {
    Obj73B0* o = arg0;
    s32 i;
    s32 found;
    s32 idx;
    Node* n;
    Node* base;
    u8 slot;

    switch (D_802194A0) {
    case 1:
    case 2:
    case 3:
        func_800A9B64(o->unk1D0, 5);
        slot = p->unk35;
        if (slot != 0x7F) {
            func_800A9B64(&D_80235F00[slot], 6);
        } else {
            for (i = 0; i < D_802194A4; i++) {
                if (getItem(i)->unk10 != o->unk1D0->unk10) {
                    func_800A9B64(getItem(i), 6);
                }
            }
        }
        break;
    case 14:
        found = 0;
        n = &D_80224EF0[D_80224E76];
        while (1) {
            if ((Part*)n != p && n->unk10 == 1) { found = 1; break; }
            if (n->next == -1) break;
            n = &D_80224EF0[n->next];
        }
        if (!found) {
            func_800A9B64(o->unk1D0, 11);
        } else if (D_8021949C == 4) {
            func_800A9B64(o->unk1D0, 0);
        } else {
            func_800A9B64(o->unk1D0, 8);
        }
        break;
    case 11:
        func_800A9B64(o->unk1D0, 8);
        break;
    }
    p->next = o->list;
    o->list = p;
    p->unk10 = 2;
    p->unk1C = o->unk0;
}
