#include "types.h"

typedef struct State8007D7C4 {
    u8 mode;
    u8 pad01;
    u16 field02;
    u32 field04;
    u32 field08;
    u8 field0C[8];
    u8 field14;
    u8 pad15;
    u16 field16;
} State8007D7C4;

extern void func_8007D470(void *state);

void Steps_InitStep_Fork(State8007D7C4 *state, u32 mode) {
    switch (mode) {
        case 0:
            break;
        case 1:
            state->mode = 1;
            state->field02 = 0;
            state->field04 = 0;
            *(u16 *)&state->field08 = 0;
            break;
        case 2:
            state->mode = 2;
            state->field02 = 0;
            state->field04 = 0;
            state->field08 = 0;
            *(u16 *)&state->field0C[0] = 0;
            *(u16 *)&state->field0C[2] = 0;
            *(u16 *)&state->field0C[4] = 0;
            state->field0C[6] = 0;
            break;
        case 3:
            state->mode = 3;
            state->field02 = 0;
            state->field08 = 0;
            state->field04 = 0;
            func_8007D470(&state->field0C);
            state->field14 = 0;
            state->field16 = 0;
            break;
        case 4:
            state->mode = 4;
            state->field02 = 0;
            state->field04 = 0;
            state->field08 = 0;
            break;
        default:
            break;
    }
}
