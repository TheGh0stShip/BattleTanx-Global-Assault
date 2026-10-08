/* ---- 0x800ED800/tu/spawn_800F1CC8.c ---- */
#include "types.h"
typedef struct { f32 x, y, z; } V3;
typedef struct {
    u8 pad[12]; V3 pos; u8 b; u8 pad19; u16 r; s32 one; s32 four; u8 pad24[4]; s32 g; s32 c; u16 m; u8 k;
} Obj;
extern s32 D_8021945C;
void *func_800A18D0(s32, s32);
u32 func_8009D914(void);
void func_800F1CC8(V3 *pos, u8 b, s32 c) {
    Obj *o = func_800A18D0(34, 56);
    if (o != 0) {
        o->four = 4;
        o->one = 1;
        o->pos = *pos;
        o->b = b;
        o->c = c;
        o->g = D_8021945C;
        o->r = func_8009D914() % 0xFFFF;
        o->m = 0xFFFF;
        o->k = 17;
    }
}

