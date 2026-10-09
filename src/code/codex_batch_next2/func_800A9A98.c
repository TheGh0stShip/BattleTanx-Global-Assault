#include "types.h"

typedef struct {
    u8 pad00[0xB];
    u8 player;
    u8 pad0C[4];
    struct {
        u8 pad00[0xC];
        s32 score;
        u8 pad10[4];
        s32 bonus;
    } *stats;
    u8 pad14[0x19C];
    u32 counter;
} CounterStateA9A98;

extern u8 D_80072D64[];
extern s32 func_8009D144(CounterStateA9A98 *state);
extern void func_800CA620(u8 player, void *message, s32 id);

void func_800A9A98(CounterStateA9A98 *state, u32 amount) {
    u32 old_group;
    u32 new_group;
    s32 difference;
    s32 bonus;

    if (func_8009D144(state)) {
        old_group = state->counter / 10000U;
        state->counter += amount;
        new_group = state->counter / 10000U;
        if (new_group != old_group) {
            func_800CA620(state->player, D_80072D64, 0x2D);
            difference = new_group - old_group;
            bonus = difference * 5;
            state->stats->score += bonus * 2;
            state->stats->bonus += bonus;
        }
    } else {
        state->counter += amount;
    }
}
