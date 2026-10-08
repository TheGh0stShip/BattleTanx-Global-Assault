/* ---- 0x800E4800/src/func_800E4ECC.c ---- */
#include "types.h"

typedef struct Obj4ECC {
    s32 active;
    s32 kind;
    u8 pad08[0x34];
    s32 field3C;
    s32 field40;
} Obj4ECC;

inline s32 func_800E4ECC(Obj4ECC* o, s32 v) {
    if (o->active == 0 || o->kind != 16) {
        return 0;
    }
    return o->field40 == v;
}

void func_800E4F00(Obj4ECC* o, s32 v) {
    if (func_800E4ECC(o, v)) {
        o->field3C = 0;
    }
}

