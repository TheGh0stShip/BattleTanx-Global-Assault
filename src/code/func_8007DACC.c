#include "types.h"

typedef struct SourceRecord8007DACC {
    u8 type;
    u8 pad01[0xD];
    u16 link_a;
    u8 pad10[6];
    u16 link_b;
} SourceRecord8007DACC;

typedef struct LookupRecord8007DACC {
    u8 type;
    u8 pad01[0xD];
    u16 link;
    u8 pad10[8];
} LookupRecord8007DACC;

extern LookupRecord8007DACC *D_80114680;

LookupRecord8007DACC *func_8007DACC(SourceRecord8007DACC *source,
                                    u16 *result) {
    LookupRecord8007DACC *record;
    u32 index;

    if (source == 0) {
        goto zero_index;
    }
    if (source->type == 2) {
        goto type_2;
    }
    if (source->type != 3) {
        goto zero_index;
    }
    index = source->link_b;
    goto have_index;
type_2:
    index = source->link_a;
    goto have_index;
zero_index:
    index = 0;

have_index:
    while (index != 0) {
        record = (LookupRecord8007DACC *)((u8 *)D_80114680 + index * 24);
        switch (record->type) {
            case 3:
                *result = index;
                return record;
            case 2:
                if (record != 0) {
                    index = record->link;
                } else {
                    index = 0;
                }
                break;
            case 4:
                goto failure;
            default:
                *result = 0;
                return 0;
        }
    }

failure:
    *result = 0;
    return 0;
}
