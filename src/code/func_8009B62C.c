#include "types.h"

typedef struct PlayerRecord {
    u8 pad00[0x10];
    s32 owner;
    u8 pad14[0x19C];
    s32 amount;
    u8 pad1B4[0x9C];
} PlayerRecord;

extern u8 D_802194A6;
extern PlayerRecord D_80235F00[];

static inline PlayerRecord *player_record_at(s32 index) {
    if (index == 0x7F) {
        return 0;
    }
    return &D_80235F00[index];
}

/* Sum the active records associated with the requested owner. */
s32 func_8009B62C(s32 owner) {
    s32 index;
    s32 total = 0;
    s32 unused;

    for (index = 0; index < D_802194A6; index++) {
        PlayerRecord *record = player_record_at(index);

        if (record->owner == owner) {
            total += record->amount;
        }
    }
    return total;
}
