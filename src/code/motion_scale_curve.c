/* RODATA_VRAM 0x80076AD0 */
#include "types.h"

extern f32 sqrtf(f32);
extern f32 D_80219488;

f32 func_800EE490(f32 duration, f32 time, f32 scale, u8 mode, u8 subtype) {
    switch (mode) {
        case 0:
            switch (subtype) {
                case 0:
                    if (time < 3000.0f) {
                        f32 root;

                        if (time < 1.0f) root = 1.0f;
                        else root = sqrtf(time);
                        return scale * D_80219488 * root / 44.721f;
                    } else if (duration - 3000.0f < time) {
                        f32 root;

                        time = duration - time;
                        if (time < 1.0f) root = 1.0f;
                        else root = sqrtf(time);
                        return scale * D_80219488 * root / 44.721f;
                    } else {
                    full_scale:
                        return scale * D_80219488;
                    }
                    break;
                case 1:
                    if (time < 3000.0f) {
                    } else if (duration - 3000.0f < time) {
                        time = duration - time;
                    } else {
                        goto negative_scale;
                    }
                    {
                        f32 root;

                        if (time < 1.0f) root = 1.0f;
                        else root = sqrtf(time);
                        return -scale * D_80219488 * root / 44.721f;
                    }
                default:
                    return 0.0f;
            }
        negative_scale:
            return -scale * D_80219488;
        case 1:
            goto full_scale;
    }
    return 0.0f;
}
