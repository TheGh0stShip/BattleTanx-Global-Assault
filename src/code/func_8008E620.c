#include "types.h"

typedef struct {
    u8 pad0[0x90];
    void *transform;
    u8 pad94[4];
    s32 index;
} ObjectIndex;

extern s32 D_80122E58[];
extern u8 D_801146D4[];

s32 func_8008E620(ObjectIndex *object, s32 mode) {
    register s32 result asm("$2");

    if (mode == 1) {
        if (object->transform != 0) {
            result = (s32)((f32)D_80122E58[object->index * 52] / 3.0f);
        } else {
            result = D_80122E58[object->index * 52];
        }
    } else {
        result = D_801146D4[mode];
    }
    return result;
}
