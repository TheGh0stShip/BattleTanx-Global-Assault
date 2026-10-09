#include "types.h"

extern f32 D_80071908;
extern f32 D_8007190C;
extern f32 D_80071910;

f32 func_8008BE74(void *object) {
    switch (*(s32 *)((u8 *)object + 0x98)) {
        case 2:
            return D_80071908;
        case 6:
            return D_8007190C;
        default:
            return D_80071910;
    }
}
