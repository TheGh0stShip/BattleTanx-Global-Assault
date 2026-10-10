/* ---- 0x800F2000/d/f2bc0.c ---- */
typedef struct { float x, y, z; } Vec3;
typedef struct { char pad[12]; unsigned short u12; char pad2[2]; } Part;
typedef struct { char pad[20]; Part *parts; char pad2[32]; } Ent;
typedef struct { int i0; Ent *ents; } Ctx;
typedef struct { short h0; unsigned short u2, u4, u6; } Desc;
typedef struct {
    char pad0[10];
    unsigned short h10;
    unsigned short h12;
    char pad14[2];
    int i16;
    int i20;
    int i24;
    int i28;
    int i32, i36, i40, i44, i48;
    Vec3 pos;
    unsigned short u64;
    unsigned char b66;
} Obj;
static const Vec3 D_80077050 = { 0.0f, 0.0f, 0.0f };
extern Obj *func_800A18D0(int, int);
extern int func_800DF758(Ctx *, int, int);
extern unsigned short func_800DF89C(Ctx *, Vec3 *, int, int, int, int, int, Obj *);
extern int func_800DF558(Ctx *, int, Vec3 *, int, int, int, int, int);
extern int func_8009D144(void);
extern int func_800E2AEC(int, Vec3 *, int, int, int, int, Obj *);
void func_800F2BC0(Desc *d, Ctx *ctx, Vec3 *pos, unsigned short ang, unsigned char b, int idx) {
    Obj *o;
    Part *part;
    Vec3 v;
    o = func_800A18D0(27, 68);
    if (o != 0) {
        o->i32 = func_800DF758(ctx, idx, d->u2);
        o->i36 = func_800DF758(ctx, idx, d->u4);
        o->i40 = func_800DF758(ctx, idx, d->u6);
        o->i44 = 60;
        o->u64 = ang;
        o->i16 = 0;
        o->i20 = 3;
        o->h12 = func_800DF89C(ctx, pos, ang, b, idx, d->u4, 2, o);
        part = &ctx->ents[idx].parts[d->u4];
        o->i28 = func_800DF558(ctx, o->i32, pos, ang, b, 2, 240, 1);
        o->b66 = b;
        o->i24 = -1000;
        o->h10 = part->u12;
        o->pos = *pos;
        if (func_8009D144()) {
            v = D_80077050;
            o->i48 = func_800E2AEC(2, &v, 0, 0, o->b66, 2, o);
        } else {
            o->i48 = 0;
        }
    }
}

