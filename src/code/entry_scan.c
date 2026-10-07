#include "types.h"

typedef struct {
    u8 active;
    u8 flags;
    u8 pad_2[0xE];
} ScanEntry;

typedef struct {
    u8 pad_0[4];
    ScanEntry* entries;
} ScanOwner;

ScanEntry* func_800BD880(ScanOwner* owner) {
    ScanEntry* entry = owner->entries;

    while (entry->active != 0) {
        if (entry->flags & 0x80) {
            return entry;
        }
        entry++;
    }
    return 0;
}
