#include "types.h"

void func_80084C50(u8* object, f32* output) {
    f32 value;

    if (*(s32*)(object + 0x184) != 0) {
        value = *(f32*)(object + 0x188);
    } else {
        value = -*(f32*)(object + 0x188);
    }
    output[1] = value;
}

extern u16 func_8009D6DC(u16 value, s32 angle);
extern void func_800909CC(void* object, s32 value, u16 first, u16 second);

void func_80084C78(u8* object, u8* input) {
    u16 angle = func_8009D6DC(*(u16*)(object + 0x20), 0x4000);

    func_800909CC(object, *(s32*)(input + 4), angle, angle);
}
