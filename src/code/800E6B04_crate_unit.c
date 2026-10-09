/* Unit 0x800E6B04..0x800E713C (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Four functions; the catalogue listed the first three as one (func_800E6B04 size 0x358):
 *   func_800E6B04 0xF4, func_800E6BF8 0xEC, func_800E6CE4 0x178, func_800E6E5C 0x2E0 (see build/fbounds.tsv).
 * Origin: claude-work/output/workers/r3/e4800/{func_800E6B04,func_800E6BF8,func_800E6CE4}.c and
 * workers/0x800E4800/r2e/func_800E6E5C_partial.c (fixed here: the colour expression is passed straight to the
 * inline helper instead of through a variable shared by the cases, strategy/CAUSES.md R22) merged into one unit; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Rodata: the two literals 25.0f and 1.0f of func_800E6B04 are the pool words at 0x80075F28..0x80075F30.
 * The next function of the original unit is probably func_800E713C (production crate_contents_spawn.c).
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800E713C */
/* RODATA_VRAM 0x80075F28 */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[0x10]; int state; int unk14; char p2[0xC]; float x; float y; float z; char p3[4]; unsigned char c34; unsigned char c35; } Obj6B04;
typedef struct { char pad[0xC]; int unkC; int state; int unk14; int unk18; int unk1C; int unk20; Vec3 pos; int unk30; unsigned char c34; unsigned char c35; unsigned char c36; char p37; short unk38; } Obj6B;

extern int D_8021945C;
extern unsigned char D_80125AB4;
extern int D_803A57A8, D_803A57AC;
extern unsigned int D_802194A0;
extern int D_803A5610;
typedef struct { short unk0; unsigned short unk2; } CrateSrc;

void func_800C8238(short, short, int, unsigned char, int, int);
unsigned char func_800AD14C(float, float, float, unsigned char);
void func_800AD6A8(int, float *, float, float, int, int, int, int, int, int, int, int);
Obj6B *func_800A18D0(int, int);
int func_8009D914(void);
short func_800B1898(Obj6B *, short, short, short, int, int, int, int, int, int, int, int, unsigned char);
int func_800DF758(int, int, unsigned short);

void func_800E6B04(Obj6B04 *arg) {
    Obj6B04 *o = arg;
    float pos[3];
    unsigned char r;
    switch (o->state) {
    case 1:
        func_800C8238(o->x, o->y, 0, o->c34, o->c35, 0);
    case 0:
        r = func_800AD14C(o->x, o->y, 30.0f, o->c34);
        if (r) {
            pos[0] = o->x;
            pos[2] = o->z + 25.0f;
            pos[1] = o->y;
            func_800AD6A8(o->unk14, pos, 1.0f, 1.0f, 0, 0, 0, 0, r, 0, 0, 0);
        }
        break;
    }
}

void func_800E6BF8(Obj6B *arg, Vec3 *v, unsigned char c) {
    Obj6B *o = arg;
    o->pos = *v;
    o->unk38 = func_800B1898(o, o->pos.x, o->pos.y, o->pos.z, -30, 30, -30, 30, -5, 55, 0, 0x20000, c);
    o->c34 = c;
    o->state = 1;
    o->unk1C = 0;
    o->unk20 = 0;
    o->unk30 = D_8021945C;
}

void func_800E6CE4(Vec3 *v, unsigned char a, unsigned char b, int d, int e) {
    Obj6B *o = func_800A18D0(7, 60);
    if (o != 0) {
        o->pos = *v;
        o->unk38 = func_800B1898(o, o->pos.x, o->pos.y, o->pos.z, -30, 30, -30, 30, -5, 55, 0, 0x20000, a);
        o->c36 = b;
        o->c35 = b;
        o->state = 1;
        o->unkC = e;
        o->unk1C = 0;
        o->unk18 = 0;
        if (D_80125AB4) {
            if (!(func_8009D914() & 1)) {
                o->unk14 = D_803A57A8;
            } else {
                o->unk14 = D_803A57AC;
            }
        } else {
            o->unk14 = d;
        }
        o->unk20 = 0;
        o->c34 = a;
    }
}

static inline void crate_spawn(Vec3 *pos, unsigned char flag, int col, int kind) {
    Obj6B *o;
    o = func_800A18D0(7, 0x3C);
    if (o != 0) {
        o->pos = *pos;
        o->unk38 = func_800B1898(o, o->pos.x, o->pos.y, o->pos.z, -30, 30, -30, 30, -5, 55, 0, 0x20000, flag);
        o->c36 = 0x7F;
        o->c35 = 0x7F;
        o->state = 1;
        o->unkC = kind;
        o->unk1C = 0;
        o->unk18 = 0;
        if (D_80125AB4 != 0) {
            if (!(func_8009D914() & 1)) {
                o->unk14 = D_803A57A8;
            } else {
                o->unk14 = D_803A57AC;
            }
        } else {
            o->unk14 = col;
        }
        o->unk20 = 0;
        o->c34 = flag;
    }
}

void func_800E6E5C(CrateSrc *src, int a1, Vec3 *pos, int a3, unsigned char flag, int a5) {
    switch (D_802194A0) {
    case 3:
        crate_spawn(pos, flag, D_803A5610, 1);
        break;
    case 14:
        crate_spawn(pos, flag, func_800DF758(a1, a5, src->unk2), 1);
        break;
    case 7:
    case 13:
        crate_spawn(pos, flag, func_800DF758(a1, a5, src->unk2), 2);
        break;
    }
}
