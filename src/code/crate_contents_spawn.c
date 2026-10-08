#include "types.h"
#define NULL 0

typedef struct Rect { u8 pad[0xC]; s16 x0; s16 y0; s16 x1; s16 y1; } Rect;
typedef struct Vec3 { f32 x, y, z; } Vec3;
typedef struct Obj {
    u8 pad0[0xC]; s32 unkC; s32 unk10; s32 unk14; s32 unk18; s32 unk1C; s32 unk20;
    Vec3 pos; u8 pad30[4]; u8 unk34; u8 unk35; u8 unk36; u8 pad37; s16 unk38;
} Obj;

extern Rect *D_80219498;
extern s16 D_80397650;
extern s32 D_803A5610;
extern u8 D_80125AB4;
extern s32 D_803A57A8;
extern s32 D_803A57AC;
extern u32 func_8009D914(void);
extern s32 func_800B205C(s32, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32, s32);
extern s32 func_800B3748(s32, s32, void *, s32, s32);
extern Obj *func_800A18D0(s32, s32);
extern s16 func_800B1898(Obj *, s16, s16, s16, s32, s32, s32, s32, s32, s32, s32, s32, s32);

void func_800E713C(void) {
    u8 buf[0x480];
    Vec3 pos;
    s16 x, y;
    s32 done = 0;
    s32 id;
    s32 col;
    Obj *o;

    do {
        x = D_80219498->x0 + func_8009D914() % (u32)(D_80219498->x1 - D_80219498->x0);
        y = D_80219498->y0 + func_8009D914() % (u32)(D_80219498->y1 - D_80219498->y0);
        id = func_800B205C(0, x, y, 0, -100, 100, -100, 100, 0, 50, 0, 0, 0) & 0xFFFF;
        D_80397650 = 0;
        if (!(func_800B3748(id, 0x0104700F, buf, 0, 1) & 0xFFFF)) {
            done = 1;
        }
    } while (done == 0);
    col = D_803A5610;
    pos.x = x;
    pos.z = 0.0f;
    pos.y = y;
    o = func_800A18D0(7, 0x3C);
    done = 2;
    if (o != NULL) {
        o->pos = pos;
        o->unk38 = func_800B1898(o, o->pos.x, o->pos.y, o->pos.z, -30, 30, -30, 30, -5, 55, 0, 0x20000, 0);
        o->unk36 = 0x7F;
        o->unk35 = 0x7F;
        o->unk10 = 1;
        o->unkC = done;
        o->unk1C = 0;
        o->unk18 = 0;
        if (D_80125AB4 != 0) {
            if (!(func_8009D914() & 1)) {
                o->unk14 = D_803A57A8;
            } else {
                o->unk14 = D_803A57AC;
            }
        } else {
            o->unk14 = col;
        }
        o->unk20 = 0;
        o->unk34 = 0;
    }
}
