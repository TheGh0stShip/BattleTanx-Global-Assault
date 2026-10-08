#include "types.h"

s32 func_8007D33C(void *data, f32 amount) {
    f32 value = *(f32 *)((u8 *)data + 4) - amount;

    *(f32 *)((u8 *)data + 4) = value;
    if (value <= 0.0f) {
        u16 old = *(u16 *)((u8 *)data + 8);
        *(f32 *)((u8 *)data + 4) = value + *(f32 *)data;
        *(u16 *)((u8 *)data + 8) = old + 1;
        if ((u16)(old + 1) >= *(u16 *)((u8 *)data + 0xA)) {
            *(u16 *)((u8 *)data + 8) = old;
            return 1;
        }
    }
    return 0;
}
