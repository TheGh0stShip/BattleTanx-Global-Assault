/* NORMALIZER_ASSISTED: exact with one label-gated assembler-hazard rule. */
typedef struct { float x, y, z; } Vec3;
typedef struct { float w[17]; } M68;
typedef struct {
    char p0[0x18]; int idx; int m1C; int m20; int m24; float f28; int i2C; char p30[0x60 - 0x30];
} TypeInfo;
typedef struct {
    char p0[0x1E4]; unsigned char r, g, b; char p1E7[0x250 - 0x1E7];
} Player;
typedef struct {
    char p0[0x18]; unsigned short a24; unsigned short a26; char p1C[0x20 - 0x1C];
    unsigned char type; unsigned char s31; unsigned char owner; char p23[0x40 - 0x23];
    int *ctl;
} Obj;
typedef struct { unsigned int a, b; } Gfx;

extern TypeInfo D_80123BB0[];
extern Player D_80235F00[];
extern void *D_803A53A0[];
static const M68 D_80075C94 = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };

extern void func_800E2E0C(Obj *, Vec3 *, unsigned short *, unsigned char *);
extern unsigned char func_800AD14C(float, float, float, int);
extern int func_800AA058(int, Vec3 *);
extern void func_8009EFD4(M68 *, float, float, float, int);
extern void func_800AD9A8(void *, int, M68 *, int, int, int, Gfx *, int);

static inline Player *getPlayer(int o) {
    if (o == 127) return 0;
    return &D_80235F00[o];
}

static inline unsigned short lerpAng(unsigned short to, unsigned short from, unsigned char t) {
    unsigned short d1 = to - from;
    unsigned short d2 = from - to;
    if (d1 < d2) return from + t * d1 / 255;
    return from - t * d2 / 255;
}

void func_800E3FDC(Obj *e) {
    TypeInfo *ti;
    Vec3 pos;
    unsigned short ang;
    unsigned char flag;
    Gfx gfx;
    unsigned char vis;
    int lod;
    Player *p;

    ti = &D_80123BB0[e->type];
    if (e->ctl != 0 && (e->ctl[0] == 0 || e->ctl[1] == 4)) return;
    func_800E2E0C(e, &pos, &ang, &flag);
    vis = func_800AD14C(pos.x, pos.y, (float)ti->i2C, flag);
    if (vis == 0) return;
    lod = func_800AA058(flag, &pos);
    p = getPlayer(e->owner);
    gfx.a = 0xFB000000;
    gfx.b = (p->r << 24) | (p->g << 16) | (p->b << 8) | 0xFF;
    if (e->s31 == 0) {
        M68 m = D_80075C94;
        func_8009EFD4(&m, pos.x, pos.z - ti->f28, pos.y, e->a26);
        func_800AD9A8(D_803A53A0[ti->idx], lod, &m, 0, 0, vis, &gfx, 1);
    } else if (e->s31 == 255) {
        M68 m = D_80075C94;
        func_8009EFD4(&m, pos.x, pos.z, pos.y, ang);
        func_800AD9A8(D_803A53A0[ti->idx], lod, &m, 0, 0, vis, &gfx, 1);
        if (ti->m1C == 263) {
            func_800AD9A8(D_803A53A0[ti->m20], lod, &m, 0, 0, vis, &gfx, 1);
            func_800AD9A8(D_803A53A0[ti->m24], lod, &m, 0, 0, vis, &gfx, 1);
        } else {
            func_800AD9A8(D_803A53A0[ti->m1C], lod, &m, 0, 0, vis, &gfx, 1);
        }
    } else {
        M68 m = D_80075C94;
        float dz = (float)e->s31 / 255.0f * ti->f28 - ti->f28;
        func_8009EFD4(&m, pos.x, pos.z + dz, pos.y, lerpAng(e->a24, e->a26, e->s31));
        func_800AD9A8(D_803A53A0[ti->idx], lod, &m, 0, 0, vis, &gfx, 1);
        if (ti->m1C == 263) {
            func_800AD9A8(D_803A53A0[ti->m20], lod, &m, 0, 0, vis, &gfx, 1);
            func_800AD9A8(D_803A53A0[ti->m24], lod, &m, 0, 0, vis, &gfx, 1);
        } else {
            func_800AD9A8(D_803A53A0[ti->m1C], lod, &m, 0, 0, vis, &gfx, 1);
        }
    }
}
