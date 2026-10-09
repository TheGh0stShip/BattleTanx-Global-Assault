/* func_800EE288 (0x800EE288-0x800EE488, 0x200): entity message handler.
 * Origin: claude-work/output/workers/r3/ed800b/f_800EE288.c (verified unchanged in this lane).
 * SPAN 0x800EE488
 */
#include "types.h"
typedef struct { u8 pad[20]; s32 p14; u8 pad18[4]; f32 x, z; u8 pad24[6]; u16 h2A; u8 b2C, b2D, b2E, b2F; } Ent;
typedef struct { u8 b0; u8 pad[3]; s32 w4; } Arg;
typedef struct { u8 flag; u8 pad[3]; f32 x, z; } Out;
void func_800EDF14(Ent *, s32, Arg *);
s32 func_8009E9C8(Arg *, f32 *);
void func_800DA4F0(s32, f32 *, u16, u8, u16, u8);
u32 func_8009D914(void);
void func_80097FB4(s32, f32, f32, f32, u8);
static inline void hit(Ent *e, Arg *arg) {
    s32 r = func_8009E9C8(arg, &e->x);
    s32 v = e->b2D;
    if (v == 0xFE) v = 0;
    if (e->b2F == 0) {
        func_800DA4F0(e->p14, &e->x, e->h2A, e->b2C, r, v);
        if (!(func_8009D914() & 1)) func_80097FB4(4, e->x, e->z, 1.0f, e->b2C);
        else func_80097FB4(40, e->x, e->z, 1.0f, e->b2C);
        e->b2F = 1;
    }
}
void func_800EE288(Ent *e_, s32 a1, u32 type, Arg *arg, Out *out) {
    Ent *e = e_;
    u8 buf[16];
    switch (type) {
    case 0:
        func_800EDF14(e, a1, arg);
        break;
    case 4:
        if (arg->w4 == 0 && e->b2C == arg->b0) {
            out->flag = 1;
            out->x = e->x;
            out->z = e->z;
        }
        break;
    case 5:
        hit(e, arg);
    case 3:
        hit(e, arg);
        break;
    }
}
