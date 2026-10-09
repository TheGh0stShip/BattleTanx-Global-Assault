#include "types.h"

typedef struct {
    f32 values[8];
} RegionShapeAD088;

typedef struct {
    u8 bytes[0x28];
} RegionEntryAD088;

extern u8 D_802194A5;
extern s32 func_800AA8A8(RegionShapeAD088 *shape, u8 kind,
                         RegionEntryAD088 *entry);

u8 func_800AD088(f32 x, f32 y, f32 z, f32 w, u8 kind) {
    RegionShapeAD088 shape;
    volatile s32 stack_pad[2];
    RegionEntryAD088 *entry;
    s32 count;
    s32 item;
    u32 result;
    register u32 saved_kind __asm__("$21") = kind;
    register u32 one __asm__("$20");
    register u8 *table __asm__("$6");

    table = &D_802194A5;
    count = *table;
    result = 0;
    item = 0;

    shape.values[0] = x;
    shape.values[1] = y;
    shape.values[2] = x;
    shape.values[3] = w;
    shape.values[4] = z;
    shape.values[5] = w;
    shape.values[6] = z;
    shape.values[7] = y;

    if (count != 0) {
        one = 1;
        entry = (RegionEntryAD088 *)(table + 0xF);
        do {
            register RegionShapeAD088 *call_shape __asm__("$4") = &shape;
            register u32 call_kind __asm__("$5");
            register RegionEntryAD088 *call_entry __asm__("$6");

            __asm__ volatile("" : "=r"(saved_kind) : "0"(saved_kind));
            call_kind = saved_kind & 0xFF;
            __asm__ volatile("" : : "r"(call_kind));
            call_entry = entry;
            if (func_800AA8A8(call_shape, call_kind, call_entry)) {
                result |= one << item;
            }
            item++;
            entry++;
        } while (item < count);
    }
    return result;
}
