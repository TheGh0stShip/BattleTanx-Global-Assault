#include "types.h"

typedef struct {
    u8 pad00[0xC0];
    u16 index;
    u8 padC2[2];
    s32 ready;
} State79FF0;

extern State79FF0 *D_801144F8;

typedef struct {
    u32 field0;
    u32 field4;
    u32 field8;
    u32 fieldC;
} Record79FF0;

void func_80079FF0(void) {
    State79FF0 *state;
    Record79FF0 *source;
    Record79FF0 *destination;

    state = D_801144F8;
    source = (Record79FF0 *)((u8 *)state + 0xC8);
    while (D_801144F8->ready != 0) {
    }
    destination = (Record79FF0 *)((state->index * 0x10) + 0x90 + (u32)state);
    source->field0 = destination->field4;
    source->field4 = destination->field8;
    source->field8 = destination->fieldC + 0x40;
}
