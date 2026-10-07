#include "types.h"

typedef struct {
    f32 x;
    f32 y;
} Vec2f;

extern void func_80089008(void* object);

void func_800859A8(u8* object, s32 first, s32 second, Vec2f* position) {
    *(s32*)(object + 0x170) = 1;
    *(s32*)(object + 0x174) = first;
    *(s32*)(object + 0x178) = second;
    *(f32*)(object + 0x17C) = position->x;
    *(f32*)(object + 0x180) = position->y;
    func_80089008(object);
}
