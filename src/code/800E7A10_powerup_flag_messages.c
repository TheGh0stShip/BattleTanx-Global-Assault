/* Unit 0x800E7A10..0x800E7DF8 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/r3/e4800/func_800E7A10.c; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800E7DF8 */
/* RODATA_VRAM 0x80075F40 */
#include "types.h"
#define NULL 0

typedef struct Team { u8 pad0[4]; u16 unk4; } Team;
typedef struct Item { u8 pad0[0xA]; u8 unkA; u8 pad0B[5]; Team *unk10; u8 pad14[0x200]; u16 unk214; u8 pad216[0x3A]; } Item;
typedef struct Src { u8 pad0[0x1D]; u8 unk1D; } Src;
typedef struct Pl { u8 pad0[0x95]; u8 unk95; u8 pad96[0x13A]; Item *unk1D0; u8 pad1D4[0xC]; s32 unk1E0; } Pl;
typedef struct Part { u8 pad0[0xC]; s32 unkC; s32 unk10; u8 pad14[4]; Src *unk18; u8 pad1C[0x1A]; u8 unk36; u8 pad37; u16 unk38; } Part;
typedef struct Msg { u8 pad0[4]; s32 type; u8 pad8[4]; Pl *pl; } Msg;

extern Item D_80235F00[];
extern s32 D_802194A0[];
extern u8 D_802194A4;
extern u8 D_802194A6[];
extern s32 func_8009D144(void);
extern void func_800B22F8(u16);
extern void func_800A9B64(void *, s32);
extern void func_80110044(char *, const char *, ...);
extern void func_800CA620(u8, char *, s32);
extern void func_800E713C(void);
extern void func_800E73B0(Pl *, Part *);
extern void func_800E8318(Src *, Part *);

static inline Item* getItem(s32 slot) {
    if (slot == 0x7F) return NULL;
    return &D_80235F00[slot];
}

static inline void markItems(Part *p) {
    s32 i;
    Item *it;
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

void func_800E7A10(Part *p, Msg *m) {
    char buf[0x80];
    Pl *pl;

    if (m->type != 4) return;
    pl = m->pl;
    if (p->unkC == 2) {
        if (func_8009D144() != 0 && pl->unk95 >= D_802194A4) return;
        func_800B22F8(p->unk38);
        p->unk38 = 0xFFFF;
        getItem(pl->unk95)->unk10->unk4++;
        p->unk10 = 3;
        if (D_802194A0[0] == 13) {
            func_800A9B64(pl->unk1D0, 8);
        }
        if (D_802194A0[0] == 2) {
            if (getItem(pl->unk95)->unk10->unk4 == 1) {
                func_80110044(buf, "GOT 1 FLAG");
            } else {
                func_80110044(buf, "GOT %d FLAGS", getItem(pl->unk95)->unk10->unk4);
            }
            func_800CA620(pl->unk95, buf, 45);
            func_800E713C();
            return;
        }
        markItems(p);
    } else {
        if (D_802194A0[0] == 14 && !(pl->unk1E0 & 2)) return;
        if (p->unk10 == 0) {
            if (getItem(p->unk18->unk1D)->unk10 == pl->unk1D0->unk10) return;
            func_800E8318(p->unk18, p);
            getItem(p->unk18->unk1D)->unk10->unk4--;
        }
        func_800B22F8(p->unk38);
        p->unk38 = 0xFFFF;
        func_800E73B0(pl, p);
        if (D_802194A0[0] == 14) {
            pl->unk1D0->unk214++;
        }
        markItems(p);
    }
}
