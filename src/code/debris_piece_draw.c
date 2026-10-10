/* ---- 0x800DA000/c/func_800DA340.c ---- */
typedef unsigned char u8; typedef unsigned short u16; typedef float f32; typedef int s32; typedef unsigned int u32;
typedef struct { f32 m[17]; } Mtx68;
typedef struct { u32 w0, w1; } GfxCmd;
typedef struct {
    u8 pad0[0xC];
    f32 pos[3];
    f32 vel[3];
    u8 pad24[0xC];
    u16 rx;
    u16 ry;
    u16 rz;
    u8 pad36[4];
    u8 unk3A;
    u8 flags;
    u8 unk3C;
    u8 r;
    u8 g;
    u8 b;
    void *model;
} Unk800DA340;
static const Mtx68 D_8007538C = { { 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f } };
extern s32 D_803A568C[];
extern u8 func_800AD14C(f32, f32, f32, u8);
extern void func_8009EF30(Mtx68 *, f32 *);
extern void func_8009F090(Mtx68 *, u16, u16, u16);
extern void func_8009FCCC(Mtx68 *, f32 *, f32 *, u16, u16, u16);
extern s32 func_800AA598(u8);
extern void func_800AE4D0(void *, s32, Mtx68 *, s32, s32, s32, u8);
extern void func_800AE7F0(void *, s32, Mtx68 *, s32, s32, u8, GfxCmd *, s32);
extern void func_800AE710(void *, s32, Mtx68 *, s32, u8, u8);

void func_800DA340(Unk800DA340 *p) {
    Mtx68 m;
    GfxCmd g;
    u8 vis;
    s32 v;

    m = D_8007538C;
    if (p->model == 0) {
        return;
    }
    vis = func_800AD14C(p->pos[0], p->pos[1], 1.0f, p->unk3C);
    if (vis == 0) {
        return;
    }
    if (p->flags & 0x40) {
        func_8009EF30(&m, p->pos);
        func_8009F090(&m, p->rz, p->ry, p->rx);
        func_800AE4D0((void*)D_803A568C[0], 0, &m, 0, 0, 0, vis);
        return;
    }
    func_8009FCCC(&m, p->vel, p->pos, p->rz, p->ry, p->rx);
    v = func_800AA598(p->unk3C);
    if (p->flags & 0x20) {
        g.w0 = 0xFB000000;
        g.w1 = (p->r << 24) | (p->g << 16) | (p->b << 8) | 0xFF;
        func_800AE7F0(p->model, v, &m, 0, 0, vis, &g, 1);
    } else {
        func_800AE710(p->model, v, &m, 0, p->unk3A, vis);
    }
}

