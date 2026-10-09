#include "types.h"

typedef struct {
    u8 bytes[0x28];
} RegionEntry800ACFE0;

extern u8 D_802194A5;
extern s32 func_800AA8A8(void *shape, u8 kind, RegionEntry800ACFE0 *entry);

u8 func_800ACFE0(void *shape, u32 kind) {
    volatile s32 stack_pad[2];
    register void *saved_shape __asm__("$21");
    register u32 result __asm__("$18");
    register s32 count __asm__("$19");
    register s32 index __asm__("$16");
    register u32 saved_kind __asm__("$20");
    register u32 one __asm__("$22");
    register RegionEntry800ACFE0 *entry __asm__("$17");
    register u8 *table __asm__("$6");

    saved_shape = shape;
    __asm__ volatile("" : "=r"(saved_shape) : "0"(saved_shape));
    result = 0;
    table = &D_802194A5;
    count = *table;
    index = 0;
    saved_kind = kind;
    if (count != 0) {
        one = 1;
        entry = (RegionEntry800ACFE0 *)(table + 0xF);
        do {
            if (func_800AA8A8(saved_shape, saved_kind, entry)) {
                result |= one << index;
            }
            index++;
            entry++;
        } while (index < count);
    }
    return result;
}
