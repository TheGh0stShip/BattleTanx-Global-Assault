/* Unit 0x800DAAE0..0x800DAC34 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/0x800DA000/b/func_800DAAE0.c; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800DAC34 */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef short s16;
typedef struct { f32 x, y, z; } Vec3f;
typedef struct {
    u8 pad0[0xC];
    Vec3f pos;
    Vec3f vel;
    f32 speed;
    s32 handle;
    s32 unk2C;
    s16 unk30;
    u8 unk32;
    u8 unk33;
    u8 unk34;
    u8 unk35;
    u8 unk36;
    u8 pad37;
    s32 unk38;
    s32 unk3C;
} Unk800DAAE0;
extern s32 D_8021945C;
extern s32 func_800A19DC(void);
extern Unk800DAAE0 *func_800A18D0(s32, s32);
extern void func_800A1AFC(s32);
extern float sqrtf(float);

void func_800DAAE0(Vec3f *pos, u8 a1, Vec3f *vel, s16 a3, u8 a4, u8 a5, s32 a6, s32 a7) {
    s32 h;
    Unk800DAAE0 *p;

    h = func_800A19DC();
    if (h == 0) {
        return;
    }
    p = func_800A18D0(38, 64);
    if (p == 0) {
        func_800A1AFC(h);
        return;
    }
    p->pos = *pos;
    p->vel = *vel;
    p->unk32 = 0;
    p->unk33 = 0;
    p->unk2C = D_8021945C;
    p->unk30 = a3;
    p->handle = h;
    p->unk36 = a1;
    p->unk34 = a4;
    p->unk35 = a5;
    p->unk38 = a6;
    p->unk3C = a7;
    p->speed = sqrtf(vel->x * vel->x + vel->z * vel->z + vel->y * vel->y);
}
