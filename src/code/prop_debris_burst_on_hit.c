/* ---- 0x800ED800/r2/f_800EE0E8.c ---- */
#include "types.h"
typedef struct { u8 pad0[20]; s32 h; u8 pad18[4]; f32 x; f32 y; u8 pad24[6]; u16 a; u8 id; u8 b; u8 pad2e; u8 done; } Obj;
extern s32 func_8009E9C8(void *, f32 *);
extern void func_800DA4F0(s32, f32 *, u16, u8, u16, u8);
extern s32 func_8009D914(void);
extern void func_80097FB4(s32, f32, f32, f32, u8);
void func_800EE0E8(Obj *o, s32 unused, void *arg) {
    s32 pad[4];
    s32 r;
    s32 b;
    r = func_8009E9C8(arg, &o->x);
    b = (o->b == 0xFE) ? 0 : o->b;
    if (o->done == 0) {
        func_800DA4F0(o->h, &o->x, o->a, o->id, r, b);
        if (!(func_8009D914() & 1)) {
            func_80097FB4(4, o->x, o->y, 1.0f, o->id);
        } else {
            func_80097FB4(40, o->x, o->y, 1.0f, o->id);
        }
        o->done = 1;
    }
}

