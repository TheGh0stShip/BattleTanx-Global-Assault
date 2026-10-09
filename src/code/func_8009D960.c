/* SPAN 0x8009DA34 */
#include "types.h"

extern s32 player_bss_0048;
extern f64 D_800725C0;
extern f32 D_800725C8;
extern f64 D_800725D0;
extern f32 D_800725D8;

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
        first_value += D_800725C0;
    }

    first_result = (f32)first_value / D_800725C8;
    second_seed = (first_seed * 0x10DCD) + 1;
    second_value = (f64)second_seed;
    player_bss_0048 = second_seed;
    if (second_seed < 0) {
        second_value += D_800725D0;
    }
    second_result = (f32)second_value / D_800725C8;
    return (first_result + second_result) * D_800725D8;
}
