/* SPAN 0x800EFB30 */
/* RODATA_VRAM 0x80076B68 */
typedef unsigned char u8; typedef unsigned short u16; typedef short s16; typedef int s32; typedef unsigned int u32; typedef float f32;
typedef struct { f32 x; f32 y; u8 pad8[2]; u8 b; u8 padb; } Pt;
typedef struct { u8 b0; u8 b1; u8 pad2[2]; s32 i4; s32 i8; u16 h0c; u16 h0e; s32 idx[6]; } Src;
typedef struct { u8 pad0[0x10]; u8 *base; u16 *verts; u8 pad18[0x38 - 0x18]; } Elem;
typedef struct { u8 pad0[4]; Elem *elems; } Ctx;
typedef struct { u8 b0; u8 b1; u16 h2; u16 h4; u16 h6; u16 h8; u16 h10; } Seg;
typedef struct { u16 h0, h2, h4, h6, h8, h10, h12, h14; } Vert;
typedef struct Node { u8 pad0[0xC]; u32 type; struct Obj *obj; struct Node *next; f32 f18; u8 pad1c[0x1C - 0x1C]; u16 h1c; s16 h1e; u16 h20; u16 h22; s32 v24; s32 v28; s32 v2c; s32 v30; s32 i34; s32 i38; u8 pad3c[0]; u16 h3c; } Node;
typedef struct Obj { u8 pad0[0xC]; f32 x; f32 y; f32 f14; s32 i18; f32 f1c; f32 f20; u16 h24; u8 b26; u8 pad27; f32 f28; f32 f2c; Node *list; u8 pad34[0]; u16 h34; u16 h36; s32 i38; u8 b3c; u8 b3d; u16 h3e; s32 i40; } Obj;
extern void *func_800A18D0(s32, s32);
extern void _bzero(void *, s32);
extern void func_800DF7A0(Ctx *, s32, s32, Pt *, s32);
extern s32 func_800DF758(Ctx *, s32, s32);
extern u16 func_8009E9C8(Pt *, Pt *);
extern f32 sqrtf(f32);

void func_800EF770(Src *src, Ctx *ctx, s32 u2, s32 u3, s32 u4, s32 n) {
    Obj *obj;
    Pt a, b;
    f32 dx, dy;
    s32 k;
    s32 off;

    obj = func_800A18D0(1, 68);
    if (obj == 0) {
        return;
    }
    obj->i40 = 0;
    _bzero(&a, 12);
    _bzero(&b, 12);
    func_800DF7A0(ctx, n, src->i4, &a, 1);
    func_800DF7A0(ctx, n, src->i8, &b, 1);
    obj->h24 = func_8009E9C8(&a, &b);
    obj->i18 = 0;
    dx = a.x - b.x;
    dy = a.y - b.y;
    obj->f14 = sqrtf(dx * dx + dy * dy);
    obj->b26 = a.b;
    obj->x = a.x;
    obj->y = a.y;
    obj->f20 = obj->f14 + 1.0f;
    obj->f1c = -1.0f;
    obj->f2c = 0;
    obj->h36 = src->h0e;
    obj->b3c = src->b1;
    off = n * 56;
    obj->b3d = 2;
    obj->h3e = 0;
    obj->f28 = 0;
    obj->h34 = src->h0c;
    obj->list = 0;
    obj->i38 = 0;
    for (k = 0; k < 6; k++) {
        Elem *e;
        Seg *s;
        Vert *v, *w;
        u32 ty;
        Node *node;
        s32 r0, r1, r2, r3;
        u16 du, dv, dw;
        s16 len;

        if (src->idx[k] == -1) {
            continue;
        }
        e = (Elem *)(off + (u8 *)ctx->elems);
        s = (Seg *)(e->base + src->idx[k]);
        v = (Vert *)((u8 *)e->verts + s->h4 * 16);
        if (s->h10 != 0xFFFF) {
            w = (Vert *)((u8 *)e->verts + s->h10 * 16);
        } else {
            w = 0;
        }
        du = v->h10 - v->h4;
        dv = v->h12 - v->h6;
        ty = s->b1;
        r0 = func_800DF758(ctx, n, s->h4);
        r1 = func_800DF758(ctx, n, s->h6);
        r2 = func_800DF758(ctx, n, s->h8);
        r3 = func_800DF758(ctx, n, s->h10);
        if (w != 0) {
            dw = w->h12 - w->h6;
        } else {
            dw = 0;
        }
        len = s->h2;
        node = func_800A18D0(2, 64);
        if (node == 0) {
            continue;
        }
        node->h22 = 0xFFFF;
        node->f18 = obj->f2c + obj->f28;
        obj->f28 += len;
        node->type = ty;
        node->h20 = dv;
        node->h1e = len;
        node->v24 = r0;
        node->v30 = r2;
        node->v28 = r1;
        node->v2c = r3;
        node->next = 0;
        node->obj = obj;
        node->h1c = du;
        node->h3c = dw;
        switch (node->type) {
        case 0:
        case 2:
        case 6:
            node->i38 = 200000;
            node->i34 = 200000;
            break;
        case 3:
            node->i38 = 100;
            node->i34 = 100;
            break;
        case 1:
        case 4:
            node->i38 = node->i34 = 20;
            break;
        }
        if (obj->list == 0) {
            obj->list = node;
        } else {
            Node *t = obj->list;
            while (t->next != 0) {
                t = t->next;
            }
            t->next = node;
        }
    }
}
