#include "types.h"

typedef struct EntryFlag {
    u8 active;
    u8 flags;
    u8 pad_2[0xE];
} EntryFlag;

void func_800BD8B8(EntryFlag* previous, EntryFlag* current) {
    previous->flags &= 0x7F;
    current->flags |= 0x80;
}

EntryFlag* func_800BD8D4(EntryFlag* current, EntryFlag* first) {
    while (current >= first) {
        if (current->flags & 0x10) {
            return current;
        }
        current--;
    }
    return 0;
}

EntryFlag* func_800BD908(EntryFlag* entry) {
    while (entry->active != 0) {
        if (entry->flags & 0x10) {
            return entry;
        }
        entry++;
    }
    return 0;
}
