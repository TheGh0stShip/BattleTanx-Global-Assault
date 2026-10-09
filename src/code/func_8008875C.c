/* SPAN 0x80088854 */
#include "types.h"

typedef struct SearchRecord8008875C {
    u8 pad000[0x10];
    s32 key;
    u8 pad014[0x23C];
} SearchRecord8008875C;

typedef struct SearchOwner8008875C {
    u8 pad000[0x1D0];
    void *descriptor;
} SearchOwner8008875C;

extern SearchRecord8008875C D_80235F00[];
extern void *func_800A9928(SearchRecord8008875C *record, s32 unused,
                           s32 mode, s32 limit, SearchOwner8008875C *owner);

void *func_8008875C(SearchOwner8008875C *owner) {
    SearchRecord8008875C *record;
    register void *result asm("$4");
    u16 index;
    s32 key;
    register u32 record_index asm("$3");

    result = 0;
    index = 0;
    key = *(s32 *)((u8 *)owner->descriptor + 0x10);
    do {
        record_index = index & 0xFFFF;
        if ((index & 0xFFFF) == 0x7F) {
            record = 0;
        } else {
            record = &D_80235F00[record_index];
        }

        if (record->key != key) {
            result = func_800A9928(record, 0, 3, 0x80, owner);
            if (result == 0) {
                result = func_800A9928(record, 0, 1, 0x80, owner);
            }
            if (result != 0) {
                break;
            }
        }
        index++;
    } while ((index & 0xFFFF) < 5);

    return result;
}
