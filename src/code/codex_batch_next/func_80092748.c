#include "types.h"

extern void func_80091768(void *object);

void func_80092748(void *owner, s32 *result) {
    void *object = *(void **)((u8 *)owner + 0xC);

    if (object == 0) {
        *result = 1;
    } else {
        func_80091768(object);
    }
}
