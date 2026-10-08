/* ---- 0x800ED800/r2/f_800EE0B0.c ---- */
#include "types.h"
typedef struct { u8 pad0[20]; s32 h; u8 pad18[4]; f32 x; f32 y; u8 pad24[6]; u16 a; u8 id; u8 b; u8 pad2e; u8 done; } Obj;
typedef struct { u8 id; u8 pad[3]; s32 busy; } Q;
typedef struct { u8 hit; u8 pad[3]; f32 x; f32 y; } R;
void func_800EE0B0(Obj *o, s32 unused, Q *q, R *r) {
    if (q->busy == 0 && o->id == q->id) {
        r->hit = 1;
        r->x = o->x;
        r->y = o->y;
    }
}

