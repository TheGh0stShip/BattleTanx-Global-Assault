typedef unsigned char u8;
typedef unsigned short u16;
typedef struct { float x, y, z; } Vec3;
typedef struct {
    u8 pad0[1]; u8 b1; u8 b2; u8 b3; u16 h4; u16 h6; int w8; u16 h12; u8 b14;
} Sub;
typedef struct {
    u8 pad0[2]; u16 h2; u16 h4; u16 h6; u16 h8; u16 h10; u8 b12; u8 pad13[3]; int w16; u8 b20;
} Desc;
typedef struct { u8 pad[12]; u16 h12; u16 pad14; } Ent16;
typedef struct { u8 pad[16]; u8 *p16; Ent16 *p20; u8 pad2[56 - 24]; } Bank;
typedef struct { int pad0; Bank *banks; } Ctx;
typedef struct {
    u8 pad0[12]; Vec3 pos; u16 h24; u8 b26; u8 b27; int w28; int w32; int w36; int w40; int w44; int w48;
    u8 b52; u8 b53; u8 b54; u8 b55; u16 h56; u8 b58;
} Obj;
extern int D_802194B0[];
extern u8 D_80219582[];
extern Obj *func_800A18D0(int, int);
extern int func_800DF758(Ctx *, int, u16);
extern u8 func_800B9C68(int);
extern u8 func_800DF63C(Ctx *, int, u16, u8, float, u8);
extern int func_800DF558(Ctx *, int, Vec3 *, u16, u8, int, int, u8);
extern u16 func_800DF89C(Ctx *, Vec3 *, u16, u8, int, u16, int, Obj *);
extern int func_800DE930(Vec3 *, u16, u8, u8, int, int, int, int, int, Sub *, int, Obj *, int, int);

void func_800E9E80(Desc *desc, Ctx *ctx, Vec3 *pos, u16 h, u8 b, int idx)
{
    Obj *obj = func_800A18D0(21, 60);
    int s0;
    u8 f;
    Sub *q;
    int v;

    if (obj != 0) {
        s0 = func_800DF758(ctx, idx, desc->h2);
        obj->w28 = func_800DF758(ctx, idx, desc->h4);
        obj->w32 = func_800DF758(ctx, idx, desc->h6);
        obj->w36 = func_800DF758(ctx, idx, desc->h8);
        obj->w40 = func_800DF758(ctx, idx, desc->h10);
        obj->b53 = 0;
        obj->b52 = 0;
        obj->pos = *pos;
        obj->h24 = h;
        obj->b26 = b;
        obj->b55 = desc->b20;
        obj->b58 = 0;
        switch (obj->b55) {
        case 1:
        case 2:
            D_802194B0[0]++;
            break;
        case 3:
            obj->b58 = func_800B9C68(***(int ***)s0);
            break;
        }
        obj->b27 = 15;
        f = func_800DF63C(ctx, idx, desc->h2, desc->b12, pos->z, b);
        obj->w44 = func_800DF558(ctx, s0, pos, h, b, 2, 240, f);
        obj->h56 = func_800DF89C(ctx, pos, h, b, idx, desc->h2, 2, obj);
        obj->b54 = *(u16 *)((u8 *)ctx->banks[idx].p20 + (desc->h8 << 4) + 12);
        if (desc->w16 == -1) {
            obj->w48 = 0;
        } else {
            q = (Sub *)(ctx->banks[idx].p16 + desc->w16);
            v = func_800DF758(ctx, idx, q->h12);
            obj->w48 = func_800DE930(pos, h, b, D_80219582[q->b3], q->h4, q->h6, q->w8, q->b1, q->b2, q, v, obj, q->b14, 1);
        }
    }
}
