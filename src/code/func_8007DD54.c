#include "types.h"

typedef struct PoolRecord8007DD54 {
    u8 type;
    u8 pad01;
    u16 field02;
    u8 pad04[8];
    u16 link_c;
    u16 link_e;
    u8 pad10[6];
    u16 link_16;
} PoolRecord8007DD54;

extern PoolRecord8007DD54 *D_80114680;
extern u16 Steps_InitStepPool(u16 *state);

u16 func_8007DD54(u16 *state, u32 selector) {
    PoolRecord8007DD54 *record;
    PoolRecord8007DD54 *next;
    u16 saved_link;
    u16 saved_field;
    u16 count;
    u32 index;
    u32 use_link_c;

    index = *state;
    if (index != 0) {
        goto have_record_index;
    }
    record = 0;
    goto got_record;
have_record_index:
    record = (PoolRecord8007DD54 *)((u8 *)D_80114680 + index * 24);
got_record:

    saved_field = record->field02;
    if (record->link_e == 0) {
        use_link_c = 1;
    } else {
        index = record->link_c;
        if (index != 0) {
            goto compare_selector;
        }
        use_link_c = 0;
        goto selected_link;
compare_selector:
        use_link_c = index == (u16)selector;
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
    next = (PoolRecord8007DD54 *)((u8 *)D_80114680 + index * 24);
got_next:
    if (next != 0) {
        next->field02 = saved_field;
    }
    return count;
}
