#include "types.h"

typedef struct {
    s32 value;
    s32 key;
} RegistryEntry;

extern RegistryEntry D_8011B0D4[];

s32 func_800C17C8(s32 key) {
    RegistryEntry* entry = D_8011B0D4;

    while ((entry->key != 0) && (entry->key != key)) {
        entry++;
    }
    return entry->value;
}
