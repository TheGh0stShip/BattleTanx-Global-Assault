/* ---- 0x800E4800/src/func_800E562C.c ---- */
#include "types.h"

typedef struct Node562C {
    u8 pad00[8];
    s16 next;
    u8 pad0A[0x14];
    u8 b1E;
    u8 pad1F[9];
    u16 snd;
    u8 pad2A[0x1A];
} Node562C;

extern s16 D_80224E88;
extern Node562C D_80224EF0[];
extern void func_800B22F8(u16);

s32 func_800E562C(void) {
    s16 i = D_80224E88;
    Node562C* p;

    if (i != -1) {
        p = &D_80224EF0[i];
        while (1) {
            if (p->snd != 0xFFFF) {
                func_800B22F8(p->snd);
                p->b1E = 4;
                return 1;
            }
            if (p->next == -1) {
                break;
            }
            p = &D_80224EF0[p->next];
        }
    }
    return 0;
}

