#include "types.h"

typedef struct RangeEntry8007D39C {
    u16 x;
    u16 y;
    u16 width;
    u16 height;
} RangeEntry8007D39C;

typedef struct Descriptor8007D39C {
    u8 pad00[8];
    u16 index;
    u8 pad0A[2];
    void **items;
    RangeEntry8007D39C *ranges;
} Descriptor8007D39C;

extern void func_8007C9B8(void *arg0, void *item, s16 x, s16 y,
                          f32 scale_x, f32 scale_y);
extern void func_8007C364(void *arg0, void *item, s16 x, s16 y,
                          u16 left, u16 top, u16 right, u16 bottom,
                          f32 scale_x, f32 scale_y);

void func_8007D39C(void *arg0, Descriptor8007D39C *descriptor,
                   s16 x, s16 y, f32 scale_x, f32 scale_y) {
    register RangeEntry8007D39C *ranges __asm__("$2");
    register void **items __asm__("$8");
    void *item;
    register RangeEntry8007D39C *range __asm__("$3");

    ranges = descriptor->ranges;
    if (ranges == 0) {
        items = descriptor->items;
        item = items[descriptor->index];
        func_8007C9B8(arg0, item, x, y, scale_x, scale_y);
    } else {
        range = &ranges[descriptor->index];
        items = descriptor->items;
        item = items[0];
        func_8007C364(arg0, item, x, y,
                      range->x, range->y,
                      range->x + range->width,
                      range->y + range->height,
                      scale_x, scale_y);
    }
}
