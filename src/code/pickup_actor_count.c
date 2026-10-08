/* ---- 0x800E4800/src/func_800E7EE0.c ---- */
#include "types.h"

typedef struct Node7EE0 {
    u8 pad00[4];
    s32 kind;
    s16 next;
    u8 pad0A[0x3A];
} Node7EE0;

extern s16 D_80235EF0;
extern s32 D_802194B0;
extern Node7EE0 D_80224EF0[];

void func_800E7EE0(void) {
    s16 i = D_80235EF0;
    s32* count = &D_802194B0;
    Node7EE0* p;

    *count = 0;
    while (i != -1) {
        p = &D_80224EF0[i];
        if (p->kind == 7) {
            (*count)++;
        }
        i = p->next;
    }
}

