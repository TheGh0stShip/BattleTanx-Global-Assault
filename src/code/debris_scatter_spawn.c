/* ---- 0x800DA000/b/func_800DA984.c ---- */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef unsigned int u32;
typedef struct { u8 b[12]; } Elem12;
typedef struct { Elem12 *elems; u8 count; } Group;
extern f32 D_800753D8, D_800753DC;
extern u32 func_8009D914(void);
extern void func_800D9D50(s32, u8, s32, u16, s32, u16, u16, u16, s32, s32, f32, Elem12 *, s32, s32, s32, s32, s32);

void func_800DA984(s32 a0, u8 a1, s32 n) {
    s32 i;

    for (i = 0; i < n; i++) {
        func_800D9D50(a0, a1, 0, 0, 0, func_8009D914() % 1280, func_8009D914() % 1280, func_8009D914() % 65535, 8192, 0, D_800753DC, 0, 0, 2, 0, 0, 0);
    }
}

