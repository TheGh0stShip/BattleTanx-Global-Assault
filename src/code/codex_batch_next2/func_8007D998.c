#include "types.h"

typedef struct PoolRecord8007D998 {
    u8 state;
    u8 pad01;
    u16 field02;
    struct PoolRecord8007D998 *next;
    u16 index;
    u8 pad0A[0xE];
} PoolRecord8007D998;

typedef struct Pool8007D998 {
    u8 state;
    u8 pad01;
    u16 field02;
    u8 pad04[0xBFFC];
    PoolRecord8007D998 *free_list;
    u16 fieldC004;
    u16 fieldC006;
    u16 fieldC008;
    u16 fieldC00A;
} Pool8007D998;

void func_8007D998(Pool8007D998 *pool) {
    register u32 offset __asm__("$2");
    register u16 index __asm__("$5");
    register u8 state __asm__("$6");

    index = 1;
    state = 1;
    pool->state = 0;
    pool->field02 = 0;
    pool->free_list = (PoolRecord8007D998 *)((u8 *)pool + 0x18);

    do {
        register PoolRecord8007D998 *record __asm__("$3");

        offset = index * 24;
        record = (PoolRecord8007D998 *)((u8 *)pool + offset);
        record->index = index;
        index++;
        offset += 24;
        record->next = (PoolRecord8007D998 *)((u8 *)pool + offset);
        record->state = state;
        record->field02 = 0;
    } while (index < 0x7FF);

    {
        register PoolRecord8007D998 *record __asm__("$2");

        record = (PoolRecord8007D998 *)((u8 *)pool + index * 24);
        record->next = 0;
        record->index = index;
        record->state = 1;
        record->field02 = 0;
    }

    pool->fieldC006 = 0xBF;
    pool->fieldC008 = 0x40;
    pool->fieldC004 = 0;
    pool->fieldC00A = 0x10;
}
