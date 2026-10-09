/* Unit 0x800DA7D0..0x800DA984 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/0x800DA000/g/a.c (func_800DA984 part dropped: production debris_scatter_spawn.c owns it); re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800DA984 */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef unsigned int u32;
typedef struct { u8 b[12]; } Elem12;
typedef struct { Elem12 *elems; u8 count; } Group;
extern f32 D_800753D8, D_800753DC;
extern u32 func_8009D914(void);
extern void func_800D9D50(s32, u8, s32, u16, s32, u16, u16, u16, s32, s32, f32, Elem12 *, s32, s32, s32, s32, s32);

void func_800DA7D0(Group **grp, s32 a1, u16 a2, u8 a3, s32 a4, u8 a5, u8 a6, u8 a7, u8 a8) {
    Group *g = *grp;
    s32 i;

    for (i = 0; i < g->count; i++) {
        func_800D9D50(a1, a3, 0, a2, 0, func_8009D914() % 1280, func_8009D914() % 1280, func_8009D914() % 65535, 8192, 0, D_800753D8, &g->elems[i], a4,
                      a5 == 0 ? ((func_8009D914() & 7) == 0) * 2 : a5, a6, a7, a8);
    }
}
