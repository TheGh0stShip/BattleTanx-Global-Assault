#include "types.h"

typedef struct HudEntry {
    u8 type;
    u8 flags;
    u8 pad_2[6];
    s32 value;
    u8 pad_C[4];
} HudEntry;

typedef struct {
    u8 pad_0[4];
    HudEntry* entries;
} HudEntryOwner;

void func_800C15B8(HudEntryOwner* owner, s32* values) {
    HudEntry* entry = owner->entries;
    u16 index;

    if (entry->type != 4) {
        do {
            entry++;
        } while (entry->type != 4);
    }

    index = 0;
    do {
        entry->value = *values++;
        if (entry->value == 0) {
            entry->flags &= 0xEF;
        } else {
            entry->flags |= 0x10;
        }
        entry++;
        entry->value = *values++;
        index++;
        entry++;
    } while (index < 9);
}
