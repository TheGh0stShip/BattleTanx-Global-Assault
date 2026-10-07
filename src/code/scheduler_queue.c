#include "types.h"

typedef struct SchedulerEntry {
    s32 field_0;
    s32 key;
    s16 next;
    u8 pad_A[0x3A];
} SchedulerEntry;

typedef struct {
    u8 pad_0[8];
    s16 entry_index;
} SchedulerOwner;

extern s16 D_80224B50;
extern SchedulerEntry D_80224EF0[];
extern s16 D_80224E68[];
extern s16 D_80235EF0;

SchedulerEntry* func_800A1A28(SchedulerOwner* owner, s32 slot) {
    s16 index;
    SchedulerEntry* entry = 0;

    if (owner == 0) {
        index = D_80224E68[slot];
    } else {
        index = owner->entry_index;
    }
    if (index != -1) {
        entry = &D_80224EF0[index];
    }
    return entry;
}

SchedulerEntry* func_800A1A80(SchedulerOwner* owner, s32 key) {
    s16 index;

    if (owner == 0) {
        index = D_80235EF0;
    } else {
        index = owner->entry_index;
    }
    while (index != -1) {
        SchedulerEntry* entry = &D_80224EF0[index];
        if (entry->key == key) {
            return entry;
        }
        index = entry->next;
    }
    return 0;
}

void func_800A1AFC(SchedulerEntry* entry) {
    s16 old_head = D_80224B50;

    entry->field_0 = 0;
    D_80224B50 = entry - D_80224EF0;
    entry->next = old_head;
}
