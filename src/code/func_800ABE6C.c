#include "types.h"

typedef struct {
    f32 weight;
    s32 value;
} WeightedValueABE6C;

extern WeightedValueABE6C D_80237180[][3][100];

s32 func_800ABE6C(s32 *output, s32 table_index) {
    s32 count = 0;
    s32 group;
    s32 item;
    s32 scan;
    s32 value;

    for (group = 0; group < 3; group++) {
        item = 0;
        do {
            if (D_80237180[table_index][group][item].weight == 1e+08f) {
                goto next_item;
            }
            value = D_80237180[table_index][group][item].value;
            for (scan = 0; scan < count; scan++) {
                if (output[scan] == value) {
                    goto next_item;
                }
            }
            output[count++] = value;
next_item:
            item++;
        } while (item < 100);
    }
    return count;
}
