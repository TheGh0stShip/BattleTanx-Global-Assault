/* Unit 0x800E16D8..0x800E18D8 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/r3/misc/func_800E16D8.c; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800E18D8 */
/* RODATA_VRAM 0x80075A30 */
typedef struct { float x, y, z; } Vec3;
typedef struct {
    char pad[0x40]; short h40; short h42; short h44; short h46; void *p48; char p4C[2]; unsigned char b4E;
} Def4;
typedef struct {
    char pad[0xC]; Def4 *def; void *p10; void *p14; unsigned short h18; unsigned short h1A;
    unsigned char b1C; unsigned char b1D; char p1E[2]; float x; float y;
} Ent4;
typedef struct { int flags; char p4[0x1C]; short h20; char p22[0x28 - 0x22]; } Slot;
extern Slot D_803978E0[];
extern float D_80075A30;
extern float D_80075A34;
extern char D_80115834[];
extern int func_8009D914(void);
extern void func_80097FB4(int, float, float, float, unsigned char);
extern void func_800DA4F0(void *, float *, unsigned short, unsigned char, unsigned short, int);
extern float func_8009D8A0(float);
extern void func_800A5BD8(Vec3 *, int, unsigned char, float, void *, int);

void func_800E16D8(Ent4 *e, unsigned short a1) {
    Vec3 v;
    int i;
    Slot *tbl;

    if (e->b1D != 0) return;
    if (!(func_8009D914() & 1)) {
        func_80097FB4(4, e->x, e->y, 1.0f, e->b1C);
    } else {
        func_80097FB4(40, e->x, e->y, 1.0f, e->b1C);
    }
    e->b1D = 1;
    e->def->b4E &= 0x7F;
    e->def->p48 = e->p14;
    { Slot *s = &D_803978E0[e->h18]; s->flags |= 0x200; }
    { Slot *s = &D_803978E0[e->h18]; s->flags &= ~0x00800400; }
    func_800DA4F0(e->p10, &e->x, e->h1A, e->b1C, a1, 0);
    for (i = 0; i < 2; ) {
        i++;
        v.x = func_8009D8A0(e->def->h44 - e->def->h40) + e->def->h40;
        v.z = func_8009D8A0(50.0f) + 25.0f;
        v.y = func_8009D8A0(e->def->h46 - e->def->h42) + e->def->h42;
        func_800A5BD8(&v, 0, e->b1C, 1.0f, D_80115834, 0);
    }
    { short two = 2; D_803978E0[e->h18].h20 = two; }
}
