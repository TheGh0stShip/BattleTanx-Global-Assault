#include "types.h"
typedef struct { u8 pad0[20]; s32 h; u8 pad18[4]; f32 x; f32 y; u8 pad24[6]; u16 a; u8 id; u8 b; u8 pad2e; u8 done; } Obj;
typedef struct { s32 pad; s32 kind; } Msg;
extern void func_800DA7D0(s32, f32 *, u16, u8, s32, s32, s32, s32, s32);
extern s32 func_8009D914(void);
extern void func_80097FB4(s32, f32, f32, f32, u8);
void func_800EDF14(Obj *o, Msg *m, s32 unused, s32 *out) {
    s32 b;
    switch (m->kind) {
    case 11: case 35: case 37: case 38: case 50:
        b = (o->b == 0xFE) ? 0 : o->b;
        if (o->done == 0) {
            func_800DA7D0(o->h, &o->x, o->a, o->id, b, 0, 0, 0, 0);
            if (!(func_8009D914() & 1)) {
                func_80097FB4(4, o->x, o->y, 1.0f, o->id);
            } else {
                func_80097FB4(40, o->x, o->y, 1.0f, o->id);
            }
            o->done = 1;
        }
        *out = 1;
        break;
    case 4:
        b = (o->b == 0xFE) ? 0 : o->b;
        if (o->done == 0) {
            func_800DA7D0(o->h, &o->x, o->a, o->id, b, 0, 0, 0, 0);
            if (!(func_8009D914() & 1)) {
                func_80097FB4(4, o->x, o->y, 1.0f, o->id);
            } else {
                func_80097FB4(40, o->x, o->y, 1.0f, o->id);
            }
            o->done = 1;
        }
        *out = 10;
        break;
    case 28:
        *out = 1;
        break;
    }
}
