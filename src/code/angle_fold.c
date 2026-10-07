#include "types.h"

extern u32 func_8009D5B4(f32 value);

u16 func_8009D578(f32 value) {
    u32 result = func_8009D5B4(value) + 0x4000;
    u32 low = result & 0xFFFF;
    register u32 limit __asm__("$5") = 0x8000;
    register u32 output __asm__("$4") = result;

    if (!(limit < low)) {
        output = limit - result;
    }
    return output & 0xFFFF;
}
