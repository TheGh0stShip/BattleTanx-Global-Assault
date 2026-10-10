#include "types.h"

typedef struct PoolRecord8007E8C8 {
    u8 type;
    u8 pad01;
    u16 field02;
    u8 pad04[0xA];
    u16 link_e;
    u8 pad10[6];
    u16 link_16;
} PoolRecord8007E8C8;

typedef struct Pool8007E8C8 {
    u8 pad00[0xC00A];
    u16 fieldC00A;
} Pool8007E8C8;

typedef struct ObjectState8007E8C8 {
    u8 pad00[0xE];
    u16 link_e;
    u16 count;
    u16 baseline;
} ObjectState8007E8C8;

extern Pool8007E8C8 *D_80114680;
extern PoolRecord8007E8C8 *Steps_InitStep_Free(u16 index);
extern void Steps_SpliceIn(u16 index);

s32 func_8007E8C8(u8 *object) {
    ObjectState8007E8C8 *state;
    PoolRecord8007E8C8 *record;
    u16 next_index;
    u16 amount;

    state = (ObjectState8007E8C8 *)(object + 0xF0);
    amount = D_80114680->fieldC00A - state->baseline;
    if (amount == 0) {
        return 0;
    }
    if (state->count < amount) {
        return 1;
    }

    do {
        record = Steps_InitStep_Free(state->link_e);
        next_index = record->field02;
        Steps_SpliceIn(state->link_e);
        state->link_e = next_index;
        record = Steps_InitStep_Free(next_index);
        if (record != 0) {
            record->link_16 = 0;
        }
        state->count--;
    } while (state->count >= amount);

    return 1;
}
