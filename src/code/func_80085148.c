#include "types.h"

extern s32 func_80084CC8(void *object, void *target);

void func_80085148(void *object) {
    if (!func_80084CC8(object, *(void **)((u8 *)object + 0xAC))) {
        func_80084CC8(object, *(void **)((u8 *)object + 0xA8));
    }
}
