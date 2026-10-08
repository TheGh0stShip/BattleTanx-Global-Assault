/* ---- 0x800E4800/b/src/func_800E56D0.c ---- */
#include "types.h"

typedef struct Src56D0 { u8 pad[4]; s32 a; s32 b; } Src56D0;
typedef struct Obj56D0 { u8 pad[0xC]; u8 a[0xC]; u8 b[0xC]; } Obj56D0;

extern Obj56D0* func_800A18D0(s32, s32);
extern void func_800DF7A0(s32, s32, s32, void*, s32);
extern void func_800B1898(Obj56D0*, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, u8);

void func_800E56D0(Src56D0* src, s32 arg1, f32* pos, u16 arg3, u8 arg4, s32 arg5) {
    Obj56D0* o;
    s32 w;

    o = func_800A18D0(17, 60);
    if (o != 0) {
        func_800DF7A0(arg1, arg5, src->a, o->a, 1);
        func_800DF7A0(arg1, arg5, src->b, o->b, 3);
        w = 140;
        if (!arg4) {
            w = 90;
        }
        func_800B1898(o, pos[0], pos[1], 0, -w, w, -60, 60, 0, 300, arg3, 0x2000, arg4);
    }
}

/* ---- 0x800E4800/b/src/func_800E5804.c ---- */
#include "types.h"

typedef struct Ctx5804 { u8 pad[0x9C]; u32 state; } Ctx5804;
typedef struct Arg5804 { u8 pad[4]; s32 kind; u8 pad8[4]; Ctx5804* ctx; } Arg5804;
typedef struct Out5804 { s32 type; s32 unk4; void* data; } Out5804;
typedef struct Obj5804 { u8 pad[0xC]; u8 data[1]; } Obj5804;

void func_800E5804(Obj5804* o, Arg5804* a, s32 unused, Out5804* out) {
    if (a->kind == 4) {
        switch (a->ctx->state) {
            case 1: case 2:
            default:
                out->type = 2;
                break;
            case 0:
                out->type = 8;
                out->data = o->data;
                break;
            case 3:
                out->type = 8;
                out->data = o->data;
                break;
        }
    } else {
        out->type = 2;
    }
}

/* ---- 0x800E4800/src/func_800E5860.c ---- */
#include "types.h"

typedef struct Sub5860 { u8 pad[0x9C]; u32 state; } Sub5860;
typedef struct Arg5860 { s32 f0; s32 kind; s32 f8; Sub5860* sub; } Arg5860;

void func_800E5860(u8* a0, Arg5860* a1, s32 a2, s32 a3, s32* out) {
    if (a2 == 0) {
        if (a1->kind == 4) {
            switch (a1->sub->state) {
            case 1:
            case 2:
                break;
            case 0:
            case 3:
                out[0] = 8;
                out[2] = (s32)(a0 + 12);
                return;
            }
        }
        out[0] = 2;
    }
}

