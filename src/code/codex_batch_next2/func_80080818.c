#include "types.h"

u8 *func_8007DA5C(u16 id);

void func_80080818(u8 *list, u16 id) {
    u8 *node = func_8007DA5C(id);
    u16 *link = (u16 *)(list + 0x104);
    f32 value = *(f32 *)(node + 8);

    while (*link != 0) {
        u8 *current = func_8007DA5C(*link);
        if (value < *(f32 *)(current + 8)) {
            break;
        }
        link = (u16 *)(current + 0x10);
        if (*link == 0) {
            break;
        }
    }
    *(u16 *)(node + 0x10) = *link;
    *link = id;
}
