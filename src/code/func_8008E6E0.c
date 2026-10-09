/* SPAN 0x8008E780 */
/* RODATA_VRAM 0x80071AF8 */
#include "types.h"

typedef struct ModeThresholdState8008E6E0 {
    u8 pad000[0x1F6];
    u16 counters[21];
} ModeThresholdState8008E6E0;

s32 func_8008E6E0(ModeThresholdState8008E6E0 *state) {
    s32 below_threshold;

    switch (*(s32 *)((u8 *)state + 0x220)) {
        case 2:
        case 3:
        case 4:
        case 8:
        case 9:
        case 14:
            below_threshold = state->counters[
                *(s32 *)((u8 *)state + 0x220)] < 15;
            break;
        case 6:
        case 15:
            below_threshold = state->counters[
                *(s32 *)((u8 *)state + 0x220)] < 60;
            break;
        case 13:
        case 16:
            below_threshold = state->counters[
                *(s32 *)((u8 *)state + 0x220)] < 6;
            break;
        case 12:
            below_threshold = state->counters[
                *(s32 *)((u8 *)state + 0x220)] < 300;
            break;
        default:
            goto return_zero;
    }

    if (below_threshold != 0) {
        goto return_zero;
    }
    return 1;

return_zero:
    return 0;
}
