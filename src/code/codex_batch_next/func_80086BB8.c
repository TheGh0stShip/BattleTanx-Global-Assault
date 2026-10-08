#include "types.h"

typedef struct {
    s32 first;
    s32 third;
    s16 second;
    s16 pad;
} StateEntry;

typedef struct {
    u8 pad[0x1C];
    StateEntry entries[4];
    u16 count;
} EntryState;

void func_80086BB8(EntryState *state, s32 first, s16 second, s32 third) {
    StateEntry *entry = &state->entries[state->count];

    entry->first = first;
    entry->second = second;
    entry->third = third;
    state->count++;
}
