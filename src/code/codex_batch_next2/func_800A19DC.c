#include "types.h"

typedef struct Func800A19DCEntry {
    u8 pad0[8];
    u16 next;
    u8 padA[58];
} Func800A19DCEntry;

extern s16 D_80224B50;
extern u8 D_80224EF0[];
extern u8 D_80224EF8[];

Func800A19DCEntry *func_800A19DC(void) {
    s32 index = D_80224B50;
    s32 offset;

    if (index == -1) return 0;
    offset = index * 68;
    D_80224B50 = *(u16 *)(D_80224EF8 + offset);
    return (Func800A19DCEntry *)(D_80224EF0 + offset);
}
