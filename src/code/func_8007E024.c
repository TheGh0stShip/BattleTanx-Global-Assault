#include "types.h"

typedef struct PoolRecord8007E024 {
    u8 type;
    u8 pad01;
    u16 field02;
    u8 pad04[8];
    u16 link_c;
    u16 link_e;
    u8 pad10[6];
    u16 link_16;
} PoolRecord8007E024;

extern PoolRecord8007E024 *D_80114680;

u16 Steps_StepPtrFromId(u16 value, u16 *state, u16 new_index) {
    PoolRecord8007E024 *record;
    PoolRecord8007E024 *old_record;
    PoolRecord8007E024 *pool;
    u16 old_index;
    u16 current_index;
    register u16 last_index __asm__("$8");
    u16 count;

    last_index = new_index;
    old_index = *state;
    {
        u32 index = last_index;

        if (index != 0) {
            goto have_new_index;
        }
        record = 0;
        goto got_new_record;
have_new_index:
        record = (PoolRecord8007E024 *)((u8 *)D_80114680 + index * 24);
    }
got_new_record:
    if (record == 0) {
        return 0;
    }

    *state = new_index;
    record->field02 = value;
    count = 1;
    current_index = record->link_16;
    if (current_index != 0) {
        pool = D_80114680;
        do {
            last_index = current_index;
            {
                u32 index = current_index;

                if (index != 0) {
                    goto have_current_index;
                }
                record = 0;
                goto got_current_record;
have_current_index:
                record = (PoolRecord8007E024 *)((u8 *)pool + index * 24);
            }
got_current_record:
            if (record == 0) {
                current_index = 0;
            } else {
                if (record->type == 2) {
                    goto type_2;
                }
                if (record->type != 3) {
                    current_index = 0;
                    goto have_next_index;
                }
                current_index = record->link_16;
                goto have_next_index;
type_2:
                current_index = record->link_e;
            }
have_next_index:
            count++;
        } while (current_index != 0);
    }

    {
        u32 index = old_index;

        record->link_16 = old_index;
        if (index != 0) {
            goto have_old_index;
        }
        old_record = 0;
        goto got_old_record;
have_old_index:
        old_record = (PoolRecord8007E024 *)((u8 *)D_80114680 + index * 24);
    }
got_old_record:
    old_record->field02 = last_index;
    return count;
}
