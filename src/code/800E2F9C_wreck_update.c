/* Unit 0x800E2F9C..0x800E3404 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32, production normalizer).
 * Origin: claude-work/output/workers/0x800E0000/r2b/f2f9c.c; re-verified in this lane with tools/kmc_cmp.py at the real address.
 * Provisional file name; the gate does not depend on it.
 */
/* SPAN 0x800E3404 */
/* RODATA_VRAM 0x80075C6C */
typedef struct { float x, y, z; } Vec3;
typedef struct { char p0[0x1D8]; float scale; char p1DC[0x250 - 0x1DC]; } Unit;
typedef struct { char p0[8]; int i8; int i12; float f16; int type; char p18[0x3C - 0x18]; Vec3 off1; Vec3 off2; char p54[0x60 - 0x54]; } Def;
typedef struct Ent Ent;
typedef struct { int id; char p4[0xB0 - 4]; Ent *owner; char pB4[0x150 - 0xB4]; float x; float y; } Target;
struct Ent { char p0[10]; unsigned char b10; unsigned char b11; char pC[0x1D - 0xC]; unsigned char b29; char p1E[2];
    unsigned char def; char p21; unsigned char unit; unsigned char b35; char p24[0x30 - 0x24]; int i48; char p34[4]; Target *target; };
typedef struct { int id; int pad[9]; } Hit;
typedef struct { float m[4][4]; } Mtx;
extern Def D_80123BB0[];
extern Unit D_80235F00[];
extern short D_80397650;
extern int D_8021945C;
extern void func_8009EEE0(void *);
extern void func_8009EFD4(void *, float, float, float, unsigned short);
extern void func_8009F288(void *, Vec3 *, Vec3 *);
extern unsigned short func_800B49E0(Vec3 *, Vec3 *, int, unsigned char, int, Ent *, Hit *);
extern float func_8009D4B0(unsigned short);
extern float func_8009D510(unsigned short);
extern void func_80097FB4(int, float, float, float, unsigned char);
extern void func_800D5C80(Vec3 *, unsigned char, float *, int *, unsigned short, unsigned char, float, int, Ent *, int, Unit *, float);

static inline Unit *getUnit(int id) { if (id == 127) return 0; return &D_80235F00[id]; }

static inline unsigned char vis_y1(int r, Hit *h, Ent *e) {
    if (r == 0) return 1;
    if (h->id != e->target->id) return 0;
    return 1;
}

void func_800E2F9C(Ent *e, Vec3 *pos, unsigned short ang, unsigned char mask) {
    Mtx m;
    Vec3 out;
    Vec3 off;
    float vel[2];
    int zero[2];
    union { Hit hit; struct { Vec3 o2; int pad; Vec3 out2; } b; } u;
    Vec3 t;
    Def *def;
    float scale;
    int fx;

    zero[0] = 0;
    zero[1] = 0;
    def = &D_80123BB0[e->def];
    scale = getUnit(e->unit)->scale;
    if (e->b11 != 0 && e->b29 == 0) return;
    off = def->off1;
    if (e->b10 & 1) off.x = -off.x;
    e->b10 ^= 1;
    func_8009EEE0(&m);
    func_8009EFD4(&m, pos->x, pos->z, pos->y, ang);
    func_8009F288(&m, &off, &out);
    if (e->b11 == 0) {
        t.x = e->target->x;
        t.y = e->target->y;
        t.z = out.z;
        D_80397650 = 1;
        e->b29 = vis_y1(func_800B49E0(&out, &t, 0x64940B, mask, 0, e, &u.hit), &u.hit, e);
        e->b11 = 16;
        if (e->b29 == 0) return;
    }
    vel[0] = func_8009D4B0(ang) * def->i12;
    vel[1] = func_8009D510(ang) * def->i12;
    fx = 0;
    if (def->type == 1) {
    } else if (def->type != 0) {
        if (def->type == 2) goto X;
    } else {
        fx = 3;
    }
J:
    func_80097FB4(fx, pos->x, pos->y, 1.0f, mask);
    func_800D5C80(&out, mask, vel, zero, ang, (unsigned int)(def->i8 * scale), def->f16, def->type, e, 0, getUnit(e->unit), def->i12 * scale);
    if (def->off2.x != 0.0f) {
        u.b.o2 = def->off2;
        if (e->b10 == 0) u.b.o2.x = -u.b.o2.x;
        func_8009F288(&m, &u.b.o2, &u.b.out2);
        func_800D5C80(&u.b.out2, mask, vel, zero, ang, (unsigned int)(def->i8 * scale), def->f16, def->type, e, 0, getUnit(e->unit), def->i12 * scale);
    }
end:
    if (e->target != 0) e->target->owner = e;
    e->i48 = D_8021945C;
    e->b35--;
    return;
X:
    fx = 18;
    goto J;
}
