#include "types.h"

typedef struct Func800A9660State {
    u8 pad0[0x10C];
    s32 expected;
    u8 enabled;
    u8 pad111[0xB3];
    s32 *current;
} Func800A9660State;

extern s32 func_800A93D0(Func800A9660State *state);
extern s32 func_800A94FC(Func800A9660State *state);

s32 func_800A9660(Func800A9660State *state) {
    if (state->enabled == 0) return -1;
    if (state->current == 0) return -1;
    if (*state->current != state->expected) {
        func_800A93D0(state);
    } else {
        func_800A94FC(state);
    }
}
