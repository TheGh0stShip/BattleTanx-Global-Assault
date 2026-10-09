#include "types.h"

s16 func_80096A54(void *context, u8 character, u16 value, s16 position,
                  s16 extra, f32 scale_x, f32 scale_y);

s16 func_80096F48(void *context, u8 *text, u16 value, s16 position,
                  s16 extra, f32 scale_x, f32 scale_y) {
    s16 width = 0;
    u8 *cursor = text;

    while (*cursor != 0) {
        width += func_80096A54(context, *cursor++, value, position + width,
                              extra, scale_x, scale_y);
    }
    return width;
}
