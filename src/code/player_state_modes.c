#include "types.h"

typedef struct PlayerStateRecord {
    u8 pad00[0x0A];
    u8 flags;
    u8 pad0B[0x69];
    s32 state;
    u8 pad78[0x1D8];
} PlayerStateRecord;

extern u8 D_802194A6[];
extern PlayerStateRecord D_80235F00[];

static inline PlayerStateRecord *player_state_record_at(s32 index) {
    if (index == 0x7F) {
        return 0;
    }
    return &D_80235F00[index];
}

void func_8009B35C(void) {
    s32 index;
    s32 unused;

    for (index = 0; index < D_802194A6[0]; index++) {
        PlayerStateRecord *record = player_state_record_at(index);

        if (record->flags & 2) {
            record->state = 2;
        }
    }
}

void func_8009B3C8(void) {
    s32 index;
    s32 unused;

    for (index = 0; index < D_802194A6[0]; index++) {
        PlayerStateRecord *record = player_state_record_at(index);

        if (record->flags & 2) {
            record->state = 1;
        }
    }
}
