#include "types.h"

typedef struct { f32 x, y, z; } V3F;
typedef struct Obj51E8 {
    u8 pad00[0x0C];
    V3F pos;
    u16 h18;
    u8 type;
    u8 b1B;
    s32 time;
} Obj51E8;
typedef struct Type51E8 {
    u8 pad00[0x18];
    s32 idx;
    s32 idx2;
    s32 idx3;
    s32 idx4;
    u8 pad28[4];
    s32 range;
    u8 pad30[0x30];
} Type51E8;
typedef struct { u32 w0, w1; } G51E8;
typedef struct { s32 m[17]; } M51E8;

extern Type51E8 D_80123BB0[];
extern s32 D_8021945C;
extern s32 D_803A53A0[];
extern const M51E8 D_80075C94;
extern u8 func_800AD14C(f32, f32, f32, s32);
extern s32 func_800AA058(s32, V3F*);
extern void func_8009EFD4(M51E8*, f32, f32, f32, u16);
extern void func_8009F824(M51E8*, f32, f32, f32);
extern void func_800AD9A8(s32, s32, M51E8*, s32, s32, s32, G51E8*, s32);

void func_800E51E8(Obj51E8* o) {
    Type51E8* t = &D_80123BB0[o->type];
    u8 r;
    s32 v;
    G51E8 dl[2];
    f32 f;

    r = func_800AD14C(o->pos.x, o->pos.y, t->range, o->b1B);
    if (r) {
        v = func_800AA058(o->b1B, &o->pos);
        if (D_8021945C - o->time > 16) {
            M51E8 m = D_80075C94;
            f = ((f32)D_8021945C - (f32)(o->time + 16)) / 16.0f;
            {
                G51E8* g = dl;
                g->w0 = 0xFB000000;
                g->w1 = ((u32)(f * 255.0f) & 0xFF) | 0x40404000;
            }
            dl[1].w0 = 0xFC167E04;
            dl[1].w1 = 0x5FFEFDFE;
            func_8009EFD4(&m, o->pos.x, o->pos.z, o->pos.y, o->h18);
            func_800AD9A8(D_803A53A0[t->idx], v, &m, 0, 0, r, dl, 2);
            if (t->idx2 == 263) {
                func_800AD9A8(D_803A53A0[t->idx3], v, &m, 0, 0, r, dl, 2);
                func_800AD9A8(D_803A53A0[t->idx4], v, &m, 0, 0, r, dl, 2);
            } else {
                func_800AD9A8(D_803A53A0[t->idx2], v, &m, 0, 0, r, dl, 2);
            }
        } else {
            M51E8 m = D_80075C94;
            G51E8* d = dl;
            f = ((f32)D_8021945C - (f32)o->time) / 16.0f;
            dl[0].w0 = 0xFB000000;
            dl[0].w1 = 0x40404000;
            dl[1].w0 = 0xFC167E04;
            dl[1].w1 = 0x5FFEFDFE;
            func_8009EFD4(&m, o->pos.x, o->pos.z, o->pos.y, o->h18);
            func_8009F824(&m, 1.0f, f, 1.0f);
            func_800AD9A8(D_803A53A0[t->idx], v, &m, 0, 0, r, d, 2);
            if (t->idx2 == 263) {
                func_800AD9A8(D_803A53A0[t->idx3], v, &m, 0, 0, r, d, 2);
                func_800AD9A8(D_803A53A0[t->idx4], v, &m, 0, 0, r, d, 2);
            } else {
                func_800AD9A8(D_803A53A0[t->idx2], v, &m, 0, 0, r, d, 2);
            }
        }
    }
}
