#include "types.h"

extern s32 D_80117EE4[];
extern s32 D_80117F04[];

s32 func_8009ACDC(s32 selection) {
    register s32 eligible __asm__("$8");
    register s32 index __asm__("$7");
    register s32 state_two __asm__("$11");
    register s32 state_one __asm__("$10");
    register s32 state_three __asm__("$9");
    register s32 *states __asm__("$6");
    register s32 *modes __asm__("$5");
    register s32 value __asm__("$3");
    register u32 is_small __asm__("$2");
    register s32 result __asm__("$2");

    eligible = 0;
    index = 0;
    state_two = 2;
    state_one = 1;
    state_three = 3;
    states = D_80117EE4;
    modes = D_80117F04;
    do {
        value = *modes;
        is_small = (u32)value < 3;
        __asm__ volatile("" : "=r"(is_small) : "0"(is_small));
        if (is_small != 0) {
          if (value != 0) {
            if (eligible == selection) {
                value = *states;
                if (value == state_two) {
                    result = 1;
                    goto done;
                }
                is_small = (u32)value < 3;
                __asm__ volatile("" : "=r"(is_small) : "0"(is_small));
                if (is_small != 0) {
                    if (value == state_one) {
                        result = 0;
                        goto done;
                    }
                } else if (value == state_three) {
                    result = 2;
                    goto done;
                }
            }
            eligible++;
          }
        }
        states++;
        modes++;
        index++;
    } while (index < 4);
    result = 0;
done:
    return result;
}
