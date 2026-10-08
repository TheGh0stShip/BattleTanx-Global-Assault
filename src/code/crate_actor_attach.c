/* ---- 0x800E4800/b/src/func_800E6540.c ---- */
#include "types.h"

typedef struct Vec3f { f32 x, y, z; } Vec3f;
typedef struct Owner6540 {
    u8 pad[0xC];
    struct Obj6540* child;
    Vec3f pos;
    u8 b1C;
    u8 b1D;
} Owner6540;
typedef struct Obj6540 {
    u8 pad[0xC];
    s32 arg;
    s32 zero10;
    s32 model;
    Owner6540* owner18;
    Owner6540* owner1C;
    s32 zero20;
    Vec3f pos;
    u8 pad30[4];
    u8 b34;
    u8 b35;
    u8 b36;
    u8 pad37;
    s16 handle;
} Obj6540;

extern Obj6540* func_800A18D0(s32, s32);
extern s32 func_8009D914(void);
extern s16 func_800B1898(Obj6540*, s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, u8);
extern u8 D_80125AB4;
extern s32 D_803A57A8;
extern s32 D_803A57AC;

void func_800E6540(Owner6540* owner, s32 model, s32 arg) {
    Obj6540* o;

    o = func_800A18D0(7, 60);
    if (o != 0) {
        o->arg = arg;
        o->zero10 = 0;
        if (D_80125AB4) {
            if (!(func_8009D914() & 1)) {
                o->model = D_803A57A8;
            } else {
                o->model = D_803A57AC;
            }
        } else {
            o->model = model;
        }
        o->b35 = o->b36 = owner->b1D;
        o->owner18 = owner;
        o->owner1C = owner;
        o->zero20 = 0;
        owner->child = o;
        o->pos = owner->pos;
        o->b34 = owner->b1C;
        o->handle = func_800B1898(o, o->pos.x, o->pos.y, o->pos.z, -30, 30, -30, 30, -5, 55, 0, 0x20000, o->b34);
    }
}

