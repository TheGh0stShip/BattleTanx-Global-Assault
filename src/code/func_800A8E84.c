#include "types.h"

typedef struct Func800A8E84Child {
    u8 pad0[0x98];
    s32 index;
} Func800A8E84Child;

typedef struct Func800A8E84Link {
    u8 pad0[4];
    s32 kind;
    u8 pad8[4];
    Func800A8E84Child *child;
} Func800A8E84Link;

typedef struct Func800A8E84State {
    u8 pad0[0x78];
    u8 object[0x94];
    Func800A8E84Link *link;
    u8 enabled;
    u8 pad111[3];
    s32 phase;
} Func800A8E84State;

extern u8 D_80121D90[];
extern u8 D_802194A5;
extern void func_800A6ADC(void *object, s32 value);

/* Exact via the label-gated allocation/expression rewrite documented in
 * docs/NORMALIZER_ASSISTED.md; the C-only output differs by eight words. */
void func_800A8E84(Func800A8E84State *state) {
    s32 next_phase;
    s32 value;
    union {
        Func800A8E84Link *link;
        s32 offset;
    } selected;

    next_phase = state->phase + 1;
    state->phase = next_phase;
    if (next_phase == 2) {
        state->phase = 0;
    }

    if (state->enabled != 0) {
        void *object;
        s32 phase;

        selected.link = state->link;
        if (selected.link->kind == 4) {
            object = state->object;
            __asm__("" : "=r"(object) : "0"(object));
            selected.offset = selected.link->child->index * 48;
            phase = state->phase * 24;
            if (D_802194A5 >= 2) {
                value = (s32)(D_80121D90 + phase + selected.offset + 0x2D0);
            } else {
                value = (s32)(D_80121D90 + phase + selected.offset);
            }
            func_800A6ADC(object, value);
        }
    }
}
