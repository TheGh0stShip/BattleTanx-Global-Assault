/* SPAN 0x8009DA34 */
#include "types.h"

extern s32 player_bss_0048;

f32 func_8009D960(void) {
    s32 first_seed;
    s32 second_seed;
    f64 first_value;
    f64 second_value;
    f32 first_result;
    f32 second_result;

    first_seed = (player_bss_0048 * 0x10DCD) + 1;
    first_value = (f64)first_seed;
    player_bss_0048 = first_seed;
    if (first_seed < 0) {
        first_value += 4294967296.0;
    }

    first_result = (f32)first_value / 4.2949673e+09f;
    second_seed = (first_seed * 0x10DCD) + 1;
    second_value = (f64)second_seed;
    player_bss_0048 = second_seed;
    if (second_seed < 0) {
        second_value += 4294967296.0;
    }
    second_result = (f32)second_value / 4.2949673e+09f;
    return (first_result + second_result) * 0.5f;
}
