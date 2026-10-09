#include "types.h"

typedef struct {
    u8 pad00[8];
    u16 links[16];
} LinkEntryB0D70;

extern LinkEntryB0D70 D_803978E0;

#define ENTRY_LINK(index, link_index) \
    ((&D_803978E0)[(index)].links[(link_index)])

s32 func_800B0D70(u16 target, u16 *head, u16 link_index) {
    u16 initial;
    u16 current;
    u16 next;
    u16 sentinel = 0xFFFF;

    initial = *head;
    if (initial == sentinel) {
        return 0;
    }
    if (initial == target) {
        u32 entry_offset = initial * 40;
        LinkEntryB0D70 *entry =
            (LinkEntryB0D70 *)((u8 *)&D_803978E0 + entry_offset);

        *head = entry->links[link_index];
        return 1;
    }
    current = initial;
    while (current != sentinel) {
        next = ENTRY_LINK(current, link_index);
        if (next == target) {
            ENTRY_LINK(current, link_index) = ENTRY_LINK(next, link_index);
            return 1;
        }
        current = next;
    }
    return 0;
}
