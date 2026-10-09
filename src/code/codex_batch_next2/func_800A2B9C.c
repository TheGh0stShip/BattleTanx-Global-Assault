#include "types.h"

typedef struct {
    void *object;
    u8 pad04[0x20];
} SearchRecordA2B9C;

typedef struct {
    u8 pad00[4];
    s32 type;
} SearchObjectA2B9C;

typedef void (*SearchCallbackA2B9C)(void *object, s32 arg1, s32 arg2,
                                    s32 arg3, s32 arg4);

typedef struct {
    SearchCallbackA2B9C callback;
    u8 pad04[8];
} SearchHandlerA2B9C;

extern u16 D_80397650;
extern SearchHandlerA2B9C D_80224B5C[];
extern u16 func_800B3748(u16 parameter, s32 mask, SearchRecordA2B9C *records,
                         s32 arg3, s32 arg4);

void func_800A2B9C(u16 parameter) {
    SearchRecordA2B9C records[32];
    volatile s32 stack_pad[2];
    register SearchRecordA2B9C *record __asm__("$16") = records;
    register s32 index __asm__("$17");
    register s32 loop_count __asm__("$18");
    register u32 count __asm__("$2");

    D_80397650 = 0;
    count = func_800B3748(parameter, 0x401100, records, 0, 0);
    if (count != 0) {
        __asm__ volatile("" : "=r"(count) : "0"(count));
        index = 0;
        if (count != 0) {
            loop_count = count;
            do {
                if (record->object != 0) {
                    if (D_80224B5C[((SearchObjectA2B9C *)record->object)->type].callback != 0) {
                        D_80224B5C[((SearchObjectA2B9C *)record->object)->type].callback(
                            record->object, 0, 2, 0, 0);
                    }
                }
                index++;
                record++;
            } while (index < loop_count);
        }
    }
}
