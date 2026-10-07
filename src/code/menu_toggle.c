#include "types.h"

typedef struct {
    u8 pad00[0x18];
    void* value;
} MenuToggleTarget;

extern s32 D_80117EC0;
extern s32 D_80117EC8;
extern u8 D_80118EDC;
extern u8 D_80118EE0;

s32 func_800C1094(void* unused, MenuToggleTarget* target) {
    void* value;

    if (D_80117EC0 == 1) {
        value = &D_80118EE0;
        D_80117EC0 = 0;
    } else {
        value = &D_80118EDC;
        D_80117EC0 = 1;
    }
    target->value = value;
    return 0;
}

s32 func_800C10DC(void* unused, MenuToggleTarget* target) {
    void* value;

    if (D_80117EC8 == 1) {
        value = &D_80118EE0;
        D_80117EC8 = 0;
    } else {
        value = &D_80118EDC;
        D_80117EC8 = 1;
    }
    target->value = value;
    return 0;
}
