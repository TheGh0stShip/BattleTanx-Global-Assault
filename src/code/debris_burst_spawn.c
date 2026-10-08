/* ---- 0x800DA000/b/func_800DA4F0.c ---- */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef unsigned int u32; typedef short s16;
typedef struct { u8 b[12]; } Elem12;
typedef struct { Elem12 *elems; u8 count; } Group;
extern f32 D_800753D0;
extern u32 func_8009D914(void);
extern void func_800D9D50(s32, u8, s32, u16, s32, u16, u16, u16, s32, s32, f32, Elem12 *, s32, s32, s32, s32, s32);

void func_800DA4F0(Group **grp, s32 a1, u16 a2, u8 a3, u16 a4, s32 a5) {
    Group *g = *grp;
    s32 i;

    for (i = 0; i < g->count; i++) {
        if (func_8009D914() & 1) {
            func_800D9D50(a1, a3, 0, a2, 0, func_8009D914() % 1280, func_8009D914() % 1280, a4, 10923, 5462, D_800753D0, &g->elems[i], a5, 0, 0, 0, 0);
        }
    }
}

