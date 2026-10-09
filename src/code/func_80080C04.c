#include "types.h"

typedef struct {
    u8 pad00[8];
    f32 x;
    f32 y;
    u8 pad10[0xFC];
    s32 mode;
    u8 pad110[8];
    f32 target_x;
    f32 target_y;
    u8 pad120[0x10];
    s16 timer;
    u8 pad132[0x3A];
    u8 flags;
} State80C04;

extern s32 func_80080CC8(State80C04 *state);
extern void func_800817E4(State80C04 *state);

void func_80080C04(State80C04 *state) {
    register f32 x __asm__("$f2");
    register f32 target_x __asm__("$f0");
    register f32 delta_x __asm__("$f4");
    register f32 y __asm__("$f2");
    register f32 delta_y __asm__("$f0");
    register u8 *target __asm__("$5") = (u8 *)state + 0xF0;
    register u8 *work __asm__("$16") = (u8 *)state + 0x128;
    __asm__ volatile("" : "=r"(target), "=r"(work) : "0"(target), "1"(work));

    if (state->mode == 2) {
        state->timer = 0;
        return;
    }

    x = state->x;
    target_x = state->target_x;
    delta_x = x - target_x;
    if (!(0.0f < delta_x)) {
        delta_x = -delta_x;
    }
    __asm__ volatile("" : : "f"(x), "f"(target_x), "f"(delta_x));
    y = state->y;
    delta_y = y - state->target_y;
    __asm__ volatile("" : : "f"(delta_y) : "memory");
    x = state->x;
    target_x = *(f32 *)(target + 0x28);
    delta_x = x - target_x;
    if (!(0.0f < delta_x)) {
        delta_x = -delta_x;
    }
    __asm__ volatile("" : : "f"(x), "f"(target_x), "f"(delta_x));
    y = state->y;
    delta_y = y - *(f32 *)(target + 0x2C);
    __asm__ volatile("" : : "f"(delta_y));

    if (state->flags & 1) {
        state->flags &= ~4;
        if (func_80080CC8(state) == 0) {
            *(s16 *)(work + 8) = 0;
        }
    } else {
        func_800817E4(state);
    }
}
