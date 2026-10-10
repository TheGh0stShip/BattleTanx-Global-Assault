#include "types.h"

/*
 * Normalizer-assisted: the C body emits the retail computation and register
 * allocation, while shape_value_decay_clamp reproduces the final three-word
 * clamp/store branch shape. See docs/NORMALIZER_ASSISTED.md.
 */

typedef struct {
    u8 pad00[0x14];
    f32 rate;
    u8 pad18[4];
    f32 value;
} ValueStateB6934;

extern f32 D_80219488;

u16 func_800B6934(ValueStateB6934 *state) {
    f32 scaled = state->value * 2e+01f;
    s32 result;
    register s32 output asm("$2");
    register f32 step asm("$f0");
    register f32 value asm("$f4");

    if (!(2.1474836e+09f <= scaled)) {
        result = (s32)scaled;
    } else {
        result = (s32)(scaled - 2.1474836e+09f) | 0x80000000;
    }
    output = result;
    value = state->value;
    if (value != 0.0f) {
        step = (state->rate + 0.0f) * D_80219488;
        if (!(step <= 0.0f)) {
            if (value > 0.0f) {
                step = value - step;
                if (step < 0.0f) {
                    step = 0.0f;
                }
            } else {
                step = value + step;
                if (step > 0.0f) {
                    step = 0.0f;
                }
            }
            state->value = step;
        }
    }
    return output;
}
