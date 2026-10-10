/* ---- 0x800F2000/d/f3098.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { float w[17]; } M44;
typedef struct { char pad[77]; unsigned char b77; } Sub;
typedef struct {
    char pad0[16];
    float f16;
    int type;
    char pad24[4];
    Sub *sub;
    int m0, m1, m2;
    char pad44[8];
    Vec3 pos;
    unsigned short u64;
    unsigned char b66;
} Obj;
extern unsigned int D_8021945C;
extern unsigned char D_8021957C[];
static const M44 D_80077080 = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };
extern float func_8009D4B0(unsigned short);
extern int func_800AA058(int, Vec3 *);
extern void func_8009EFD4(M44 *, float, float, float, int);
extern void func_800AE4D0(int, int, M44 *, int, int, int, int);
void func_800F3098(Obj *o) {
    M44 m = D_80077080;
    int vis;
    int h;
    vis = D_8021957C[o->b66] & (o->sub->b77 >> 4);
    if (vis) {
        o->pos.z = o->f16 + func_8009D4B0(D_8021945C * 1310) * 6.0f;
        func_8009EFD4(&m, o->pos.x, o->pos.z, o->pos.y, o->u64);
        h = func_800AA058(o->b66, &o->pos);
        if (o->type == 4) {
            func_800AE4D0(o->m2, h, &m, 0, 0, 0, vis);
        } else {
            func_800AE4D0(o->m0, h, &m, 0, 0, 0, vis);
            func_800AE4D0(o->m1, h, &m, 0, 0, 0, vis);
        }
    }
}

