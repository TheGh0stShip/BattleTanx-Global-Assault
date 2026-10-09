#include "types.h"

extern u8 D_802194A4;
extern s32 D_80216FA0[];
extern s32 D_80216FC0[];

extern void func_80098CC8(void);

s32 func_80098F24(void) {
    volatile s32 stack_pad[2];
    register s32 result __asm__("$6");
    register s32 index __asm__("$5");
    register s32 sentinel __asm__("$8");
    register u8 *count __asm__("$7");
    register s32 *state __asm__("$4");
    register s32 *values __asm__("$3");
    register u8 *count_source __asm__("$3");
    register s32 count_value __asm__("$2");
    s32 value;

    func_80098CC8();
    count_source = &D_802194A4;
    count_value = *count_source;
    __asm__ volatile("" : "=r"(count_value) : "0"(count_value));
    result = 1;
    index = 0;
    if (count_value > 0) {
        sentinel = -1;
        __asm__ volatile("" : : "r"(sentinel));
        count = count_source;
        __asm__ volatile("" : "=r"(count) : "0"(count));
        state = D_80216FC0;
        values = D_80216FA0;
loop:
        if (*state != 0) {
            value = *values;
            if (value != sentinel) {
                values++;
                goto update;
            }
            result = 0;
            goto done;
        }
        value = *values++;
update:
        *state = (u32)~value > 0;
        index++;
        state++;
        if (index < *count) {
            goto loop;
        }
    }
done:
    return result;
}
