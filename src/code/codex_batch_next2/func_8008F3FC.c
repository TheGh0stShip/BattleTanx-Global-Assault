#include "types.h"

typedef struct {
    s32 value;
    u8 pad04[0x20];
} SearchRecord8F3FC;

extern u16 D_80397650;
extern u16 func_800B3748(u16 parameter, s32 mask, SearchRecord8F3FC *records,
                         s32 arg3, s32 mode);

s32 func_8008F3FC(u16 parameter, s32 mask, s32 value, s32 unused) {
    SearchRecord8F3FC records[32];
    volatile s32 stack_pad[2];
    register SearchRecord8F3FC *buffer __asm__("$17");

    if (value != 0) {
        buffer = records;
        D_80397650 = 0;
        {
            register s32 count __asm__("$4");
            register s32 index __asm__("$3");
            register SearchRecord8F3FC *entry __asm__("$6");

            count = func_800B3748(parameter, mask, buffer, 0, 0);
            if (count != 0) {
                index = 0;
                entry = buffer;
                do {
                    if (entry->value != value) {
                        return 1;
                    }
                    index++;
                    entry++;
                } while (index < count);
            }
        }
        return 0;
    }
    D_80397650 = 0;
    return func_800B3748(parameter, mask, records, 0, 1) != 0;
}
