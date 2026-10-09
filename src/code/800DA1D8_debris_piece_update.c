/* Unit 0x800DA1D8..0x800DA340 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/0x800DA000/b/func_800DA1D8.c; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800DA340 */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef short s16;
typedef struct {
    u8 pad0[0xC];
    f32 pos[3];
    u8 pad18[0xC];
    f32 vx;
    f32 vz;
    f32 vy;
    u16 unk30;
    u16 unk32;
    u16 pad34;
    u16 unk36;
    u16 unk38;
    u8 pad3A;
    u8 flags;
    u8 unk3C;
} Unk800DA1D8;
extern f32 D_80075388;
extern u8 D_80115C38[];
extern s32 func_8009D914(void);
extern void func_800A5BD8(f32 *, s32, s32, f32, void *, s32);

void func_800DA1D8(Unk800DA1D8 *p, s32 *done) {
    if (p->flags & 8) {
        if ((func_8009D914() & 0xF) == 0) {
            p->vx = -p->vx;
            p->vz = -p->vz;
        }
    } else {
        p->vy -= D_80075388;
    }
    p->pos[0] += p->vx;
    p->pos[2] += p->vy;
    p->pos[1] += p->vz;
    if (p->flags & 2) {
        func_800A5BD8(p->pos, 0, p->unk3C, 1.0f, D_80115C38, 0);
    }
    if (p->vy < 0.0f) {
        if (p->flags & 1) {
            *done = 1;
            return;
        }
        if (!(p->flags & 0x4A) && (func_8009D914() & 7) == 0) {
            *done = 1;
            return;
        }
    }
    if (p->pos[2] > 0.0f) {
        p->unk30 += p->unk36;
        p->unk32 += p->unk38;
    } else {
        *done = 1;
    }
}
