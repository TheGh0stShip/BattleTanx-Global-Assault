#include "types.h"

typedef struct PoolRecord8007DE3C {
    u8 type;
    u8 pad01;
    u16 field02;
    u8 pad04[8];
    u16 link_c;
    u16 link_e;
    u8 pad10[6];
    u16 link_16;
} PoolRecord8007DE3C;

extern PoolRecord8007DE3C *D_80114680;
extern s32 Steps_InitStepPool(u16 *state);

u16 Steps_GetNextStepId(u16 *state) {
    PoolRecord8007DE3C *record;
    PoolRecord8007DE3C *next;
    u16 saved_link;
    register u16 saved_field __asm__("$18");
    s32 count;
    u32 total;
    u32 index;
    u32 link_c;
    u32 use_link_c;
    register s32 done __asm__("$19");

    total = 0;
    done = 0;
    do {
        index = *state;
        if (index != 0) {
            goto have_record_index;
        }
        record = 0;
        goto got_record;
have_record_index:
        record = (PoolRecord8007DE3C *)((u8 *)D_80114680 + index * 24);
got_record:
        if (record == 0) {
            done = 1;
        } else {
            switch (record->type) {
                case 3:
                    state = &record->link_16;
                    break;
                case 2:
                    index = *state;
                    if (index != 0) {
                        goto have_unlink_index;
                    }
                    record = 0;
                    goto got_unlink_record;
have_unlink_index:
                    record = (PoolRecord8007DE3C *)((u8 *)D_80114680 + index * 24);
got_unlink_record:
                    saved_field = record->field02;
                    if (record->link_e == 0) {
                        use_link_c = 1;
                    } else {
                        link_c = record->link_c;
                        if (link_c != 0) {
                            goto compare_zero;
                        }
                        use_link_c = 0;
                        goto selected_link;
compare_zero:
                        use_link_c = link_c == 0;
                    }
selected_link:
                    if (use_link_c != 0) {
                        saved_link = record->link_c;
                        record->link_c = 0;
                    } else {
                        saved_link = record->link_e;
                        record->link_e = 0;
                    }
                    count = Steps_InitStepPool(state);
                    *state = saved_link;
                    index = saved_link;
                    if (index != 0) {
                        goto have_next_index;
                    }
                    next = 0;
                    goto got_next;
have_next_index:
                    next = (PoolRecord8007DE3C *)((u8 *)D_80114680 + index * 24);
got_next:
                    if (next != 0) {
                        next->field02 = saved_field;
                    }
                    total += count & 0xFFFF;
                    break;
                case 4:
                    goto stop;
                default:
                    done = 1;
                    break;
            }
        }
        continue;
stop:
        done = 1;
    } while (done == 0);

    return (u16)total;
}
