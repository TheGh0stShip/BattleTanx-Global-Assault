/* RODATA_VRAM 0x800715F8 */
#include "types.h"

f32 func_80087798(f32 start, f32 value, f32 limit) {
    f32 result;

    if (value <= limit) {
        if (start <= value) {
            result = 0.0f;
        } else if (limit <= start) {
            result = 1.0f;
        } else {
            result = (start - value) / (limit - value);
        }
    } else if (start <= limit) {
        result = 1.0f;
    } else if (value <= start) {
        result = 0.0f;
    } else {
        result = (start - limit) / (value - limit);
    }
    return result;
}
