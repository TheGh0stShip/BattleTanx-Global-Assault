/* ---- 0x800D0000/func_800D0960.c ---- */
unsigned int func_800D0960(unsigned int a) {
    return (a & 0x7C0) >> 6;
}

/* ---- 0x800D0000/func_800D096C.c ---- */
typedef unsigned short u16;
typedef struct { char pad[0xE]; u16 flags; } Ent10;
extern Ent10 D_803A6FD8[];

Ent10 *func_800D096C(void) {
    u16 i;
    for (i = 0; i < 4; i++) {
        if (!(D_803A6FD8[i].flags & 0x8000)) break;
    }
    if (i >= 4) return 0;
    D_803A6FD8[i].flags = 0x8000;
    return &D_803A6FD8[i];
}

