#include "types.h"

typedef struct {
    u32 key;
    u32 unused;
    u32 value;
} TableEntry;

typedef struct {
    u8 pad00[0xC10];
    s32 count;
    TableEntry entries[1];
} TableAContainer;

typedef struct {
    u8 pad00[0x1818];
    s32 count;
    TableEntry entries[1];
} TableBContainer;

typedef struct {
    u8 pad00[8];
    s32 count;
    TableEntry entries[1];
} TableCContainer;

u32 func_800B9D4C(TableAContainer* table, u32 key) {
    s32 i;
    for (i = 0; i < table->count; i++) {
        if (table->entries[i].key == key) {
            return table->entries[i].value;
        }
    }
    return 0;
}

u32 func_800B9D94(TableBContainer* table, u32 key) {
    s32 i;
    for (i = 0; i < table->count; i++) {
        if (table->entries[i].key == key) {
            return table->entries[i].value;
        }
    }
    return 0;
}

u32 func_800B9DDC(TableCContainer* table, u32 key) {
    s32 i;
    for (i = 0; i < table->count; i++) {
        if (table->entries[i].key == key) {
            return table->entries[i].value;
        }
    }
    return 0;
}
