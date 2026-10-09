#include "types.h"

extern s32 D_8021945C;

s32 func_80096250(void *object) {
    s32 flags = *(s32 *)((u8 *)object + 0x1E0);
    s32 stamp;

    if (flags & 8) {
        return 0;
    }
    if (flags & 0x1000) {
        return 1;
    }
    stamp = *(s32 *)((u8 *)object + 0x260);
    if (stamp != -1) {
        return D_8021945C - stamp < 45;
    }
    return 1;
}
