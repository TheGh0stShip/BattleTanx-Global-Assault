#include "types.h"

extern s32 func_80081834(void *object);
extern s32 func_80081EA4(void *object);
extern void func_8007E988(void *object);
extern void func_8008268C(void *object);

void func_800817E4(void *object) {
    if (!func_80081834(object) && !func_80081EA4(object)) {
        func_8007E988(object);
        func_8008268C(object);
    }
}
