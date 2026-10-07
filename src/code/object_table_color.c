#include "types.h"

typedef struct {
    u8 red;
    u8 green;
    u8 blue;
    u8 pad_3[0x109];
} ObjectTableEntry;

extern ObjectTableEntry D_80236B30[];

void func_800A9BF0(u8 index, u8 red, u8 green, u8 blue) {
    ObjectTableEntry* entry = &D_80236B30[index];

    entry->red = red;
    entry->green = green;
    entry->blue = blue;
}
