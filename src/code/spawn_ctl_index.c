/* ---- 0x800D0000/b/func_800D0814.c ---- */
typedef unsigned char u8;
typedef struct { char pad[0x10]; } Ent16;
extern Ent16 D_803A66C0[];

u8 func_800D0814(Ent16 *p) {
    u8 i;
    for (i = 0; i < 50; i++) {
        if (p == &D_803A66C0[i]) return i;
    }
    return 0xFF;
}

