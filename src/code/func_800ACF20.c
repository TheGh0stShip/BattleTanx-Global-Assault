#include "types.h"

typedef struct {
    u8 bytes[0x28];
} RegionShapeACF20;

typedef struct {
    u8 bytes[0x28];
} RegionEntryACF20;

extern RegionShapeACF20 D_803978E0[];
extern u8 D_802194A5;
extern void func_800B2BE4(RegionShapeACF20 *source, RegionShapeACF20 *destination);
extern s32 func_800AA8A8(RegionShapeACF20 *shape, u8 kind,
                         RegionEntryACF20 *entry);

u8 func_800ACF20(u16 index, u32 kind) {
    RegionShapeACF20 shape;
    RegionEntryACF20 *entry;
    s32 count;
    s32 item;
    u32 result;
    register u32 one __asm__("$21");
    register u8 *table __asm__("$6");

    func_800B2BE4(&D_803978E0[index], &shape);
    table = &D_802194A5;
    __asm__ volatile("" : "=r"(table) : "0"(table));
    count = *table;
    result = 0;
    item = 0;
    if (count != 0) {
        one = 1;
        entry = (RegionEntryACF20 *)(table + 0xF);
        do {
            if (func_800AA8A8(&shape, kind, entry)) {
                result |= one << item;
            }
            item++;
            entry++;
        } while (item < count);
    }
    return result;
}
