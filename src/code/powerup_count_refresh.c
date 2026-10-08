/* ---- 0x800E4800/src/func_800E7DF8.c ---- */
#include "types.h"

extern void func_800E7A10(s32, s32, s32, s32);

void func_800E7DF8(s32 a0, s32 a1, s32 a2, s32 a3, s32 a4) {
    if (a2 == 0) {
        func_800E7A10(a0, a1, a3, a4);
    }
}

/* ---- 0x800E4800/b/src/func_800E7E24.c ---- */
#include "types.h"

typedef struct Cnt { u8 pad[4]; u16 n; } Cnt;
typedef struct Item { u8 pad[0x10]; Cnt* cnt; u8 pad14[0x23C]; } Item;
typedef struct Info { u8 pad[0x1D]; u8 slot; } Info;
typedef struct Node7E24 {
    u8 pad00[4];
    s32 kind;
    s16 next;
    u8 pad0A[6];
    s32 busy;
    u8 pad14[4];
    Info* info;
    u8 pad1C[0x28];
} Node7E24;

extern s16 D_80235EF0;
extern Node7E24 D_80224EF0[];
extern Item D_80235F00[];

static inline Item* getItem(s32 s) {
    if (s == 0x7F) return 0;
    return &D_80235F00[s];
}
void func_800E7E24(void) {
    s16 i = D_80235EF0;
    Node7E24* p;
    while (i != -1) {
        p = &D_80224EF0[i];
        if (p->kind == 7 && p->busy == 0) {
            getItem(p->info->slot)->cnt->n++;
        }
        i = p->next;
    }
}

