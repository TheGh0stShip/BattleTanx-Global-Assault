#include "types.h"

typedef struct {
    u8 bytes[0x1C];
} Entry80086700;

typedef struct {
    u32 mask;
    u8 pad_04[0x38];
    f32 values[6];
} Data80086700;

typedef struct {
    u8 pad_00[0x10];
    Entry80086700 *entry;
    Data80086700 data;
} Object80086700;

extern Entry80086700 D_80236A90[];

extern void func_8008723C(Object80086700 *object, s32 arg1, s32 arg2, s32 arg3);

void func_80086700(Object80086700 *object) {
    register Data80086700 *data __asm__("$7");
    register Entry80086700 *entry __asm__("$8");
    register u16 index __asm__("$5");
    register u32 mask __asm__("$6");
    register Entry80086700 *entries __asm__("$9");
    register u32 index_value __asm__("$3");
    register u32 result_mask __asm__("$2");
    register Entry80086700 *candidate __asm__("$2");
    register u32 offset __asm__("$2");

    data = &object->data;
    entry = object->entry;
    index = 0;
    mask = 0x08000000;
    entries = D_80236A90;
    while (index < 5) {
        index_value = index;
        __asm__ volatile("" : "=r"(index_value) : "0"(index_value));
        offset = index_value * sizeof(Entry80086700);
        __asm__ volatile("" : "=r"(offset) : "0"(offset));
        candidate = (Entry80086700 *)(offset + (u32)entries);
        if (entry == candidate) {
            result_mask = mask;
            goto found;
        }
        index++;
        mask <<= 1;
    }
    result_mask = 0xF8000000;

found:
    data->mask = result_mask;
    data->values[0] = 1000.0f;
    data->values[1] = -1200.0f;
    data->values[2] = 1400.0f;
    data->values[3] = -2400.0f;
    data->values[4] = 800.0f;
    data->values[5] = -700.0f;
    func_8008723C(object, 0, 0, 0);
}
