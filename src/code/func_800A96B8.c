#include "types.h"

typedef struct Func800A96B8Inner {
    u8 pad0[0xC];
    s32 value;
} Func800A96B8Inner;

typedef struct Func800A96B8State {
    u8 pad0[0xA];
    u8 flags;
    u8 padB[5];
    Func800A96B8Inner *inner;
    u8 pad14[0x60];
    s32 result;
    u8 pad78[0x150];
    u8 field1C8;
    u8 field1C9;
    u8 pad1CA;
    u8 field1CB;
    u8 pad1CC[0x30];
    u8 field1FC;
} Func800A96B8State;

extern s32 func_8009D144(Func800A96B8State *state);

void func_800A96B8(Func800A96B8State *state) {
    if (func_8009D144(state) == 0) return;
    if (state->flags & 2) {
        if (state->field1C8 + state->field1CB != 0) {
            state->result = 0;
            return;
        }
        if (state->inner->value >= 5) {
            state->result = 0;
            return;
        }
        state->result = 2;
        return;
    }
    if (state->field1C9 + state->field1CB != 0) return;
    if (state->field1FC != 0) return;
    state->result = 2;
}
