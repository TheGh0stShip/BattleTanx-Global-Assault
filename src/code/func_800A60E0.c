#include "types.h"

extern void *D_80114E94[];
extern void *D_80114EA0;
extern s32 func_800EC1F8(void *state, u8 mode, s32 arg2, s32 arg3,
                         s32 arg4, s32 arg5);
extern void func_800A5BD8(void *state, u16 value, u8 mode, f32 scale,
                         void *event, s32 arg5);

void func_800A60E0(void *state, u16 value, u8 mode, s32 category,
                   void *argument) {
    void *event = D_80114E94[category];
    s32 result;

    if (event == 0) {
        return;
    }
    if (category == 3) {
        result = func_800EC1F8(state, mode, 0x7F, 0x28, 1, 0);
        func_800A5BD8(state, value, mode, 1.0f, D_80114EA0, result);
    } else {
        func_800A5BD8(state, value, mode, 1.0f, event, (s32)argument);
    }
}
