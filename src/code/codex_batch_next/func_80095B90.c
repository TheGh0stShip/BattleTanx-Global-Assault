#include "types.h"

extern s32 func_800979F4(s32 argument);
extern s32 func_80097FB4(s32, s32, s32, s32, s32);

s32 func_80095B90(void *object, s32 argument) {
    u8 type;
    register s32 scale __asm__("$7");

    if (*(s32 *)((u8 *)object + 0x1E0) & 2) {
        return func_800979F4(argument);
    }
    type = *(u8 *)((u8 *)object + 0x94);
    scale = 0x3F800000;
    __asm__ volatile("" : "=r"(scale) : "0"(scale));
    func_80097FB4(argument, *(s32 *)((u8 *)object + 8),
                  *(s32 *)((u8 *)object + 0xC), scale, type);
    return 0;
}
