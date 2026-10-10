#include "types.h"

typedef struct PoolRecord8007D884 {
    u8 mode;
    u8 pad01;
    u16 field02;
    struct PoolRecord8007D884 *next;
    u16 index;
    u16 field0A;
    u16 field0C;
    u16 field0E;
    u16 field10;
    u8 field12;
    u8 pad13;
    u8 field14;
    u8 pad15;
    u16 field16;
} PoolRecord8007D884;

typedef struct Pool8007D884 {
    u8 pad00[0xC000];
    PoolRecord8007D884 *free_list;
} Pool8007D884;

extern Pool8007D884 *D_80114680;
extern void func_8007D470(void *state);

u16 Steps_InitStep_Leg(PoolRecord8007D884 **output, u32 mode) {
    Pool8007D884 *pool = D_80114680;
    PoolRecord8007D884 *record = pool->free_list;
    u16 index;

    if (record == 0) {
        if (output != 0) {
            *output = 0;
        }
        return 0;
    }

    index = record->index;
    pool->free_list = record->next;
    record->mode = mode;

    switch (mode) {
        case 2:
            record->mode = 2;
            record->field02 = 0;
            record->next = 0;
            *(u32 *)&record->index = 0;
            record->field0C = 0;
            record->field0E = 0;
            record->field10 = 0;
            record->field12 = 0;
            break;
        case 3:
            record->mode = 3;
            record->field02 = 0;
            *(u32 *)&record->index = 0;
            record->next = 0;
            func_8007D470(&record->field0C);
            record->field14 = 0;
            record->field16 = 0;
            break;
        case 4:
            record->mode = 4;
            record->field02 = 0;
            record->next = 0;
            *(u32 *)&record->index = 0;
            break;
        default:
            break;
    }

    if (output != 0) {
        *output = record;
    }
    return index;
}
