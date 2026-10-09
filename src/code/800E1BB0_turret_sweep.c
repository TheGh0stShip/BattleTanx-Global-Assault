/* Unit 0x800E1BB0..0x800E2018 (BattleTanx GA, KMC GCC 2.7.2 -O2 -G0 -mips3 -mgp32 -mfp32).
 * NORMALIZER-ASSISTED: the C below is exact only with the proposed assembler-hazard rules V4_X (second nop after a hoisted mul.s whose result the next insn reads) and V4_Y (label after an FP load)
 * (see NORMALIZER_ASSISTED.tsv). With the production normalizer it differs (see comparisons/).
 * Origin: claude-work/output/workers/r3/e1bb0_e5bb8/src/func_800E1BB0.c
 * Not integration-ready until the rule is adopted; the C itself is a complete source match otherwise.
 */
/* SPAN 0x800E2018 */
/* RODATA_VRAM 0x80075C40 */
/* LDSYM func_80108DC0=0x80108DC0 */
typedef struct { float m[4][4]; } Mtx;
typedef struct { char p0[0x48]; int i72; unsigned char b76; unsigned char b77; } Obj;
typedef struct { char p0[0xC]; Obj *obj; char p10[4]; int i20; char p18[8]; float radius; float x; float y; float z;
    char p30[2]; unsigned short ang50; unsigned short ang52; unsigned char b54; unsigned char b55; unsigned char b56; unsigned char b57; } Ent;
typedef struct { int id; char p4[4]; float x; float y; char p10[4]; float f20; float f24; char p1C[0x28 - 0x1C]; } Hit;
typedef struct { float x, y, z; } Vec3;
extern int D_8021945C;
extern short D_80397650;
extern float func_8009D4B0(unsigned short);
extern float func_8009D510(unsigned short);
extern int func_8009D5B4(float);
extern unsigned short func_8009D578(float);
extern unsigned short func_800B49E0(void *, void *, int, unsigned char, int, int, Hit *);
extern void func_8009EEE0(Mtx *);
extern void func_8009EF4C(Mtx *, float, float, unsigned short);
extern void func_80108DC0(Mtx *, Obj *);
extern void func_800A2C6C(unsigned char, Vec3 *, int, int, int, int);

static inline unsigned short coll(void *a, void *b, int c, unsigned char d, int e, int f, Hit *g) {
    D_80397650 = 0;
    return func_800B49E0(a, b, c, d, e, f, g);
}
#define ABS(x) ((x) > 0.0f ? (x) : -(x))

void func_800E1BB0(Ent *e, int *out) {
    Hit hit;
    float p[2];
    Vec3 q;
    Mtx m;
    float r;
    float dx, dy, mx, mn;
    int a;

    if (e->b56 == 0 && e->b57 != 0) {
        a = D_8021945C % e->b57;
        e->obj->b77 &= 0xF0;
        e->obj->b77 |= a;
    }
    if (e->b56 == 2) {
    r = func_8009D4B0(e->ang52 += 0x800) * e->radius;
    p[0] = e->x + r * func_8009D4B0(e->ang50);
    p[1] = e->y + r * func_8009D510(e->ang50);
    if (coll(&e->x, p, 1027, e->b54, 0, 0, &hit)) {
        if (e->b55 == 0) {
            if (hit.f20 < 0.0f) a = -func_8009D5B4(hit.f24);
            else a = func_8009D5B4(hit.f24);
            if ((unsigned short)((int)(e->ang50 - (a | (int)0xFFFF0000)) % 0x10000) > 0x8000) e->b55 = 1;
            else e->b55 = 2;
        }
        p[0] = e->x + r * func_8009D4B0(e->ang50 = (a = e->ang50, e->b55 == 1) ? a + 1024 : a - 1024);
        p[1] = e->y + r * func_8009D510(e->ang50);
            if (coll(&e->x, p, 1027, e->b54, 0, 0, &hit)) {
            dx = hit.x - e->x;
            dy = hit.y - e->y;
mx = ((ABS(dx) > ABS(dy)) ? ABS(dx) : ABS(dy)) + ((ABS(dx) < ABS(dy)) ? ABS(dx) : ABS(dy)) * 3.0f / 8.0f; e->ang52 = func_8009D578(mx / e->radius);
        }
    }
    if (e->ang52 >= 0x4000) {
        *out = 1;
        e->obj->b76 = 0;
        e->obj->i72 = e->i20;
        func_8009EEE0(&m);
        func_8009EF4C(&m, e->x, e->y, e->ang50);
        func_80108DC0(&m, e->obj);
        q.x = p[0];
        q.y = p[1];
        q.z = e->z;
        func_800A2C6C(e->b54, &q, 100, 1, 127, 256);
    }
    e->obj->b77 |= 0xF0;
    }
}
