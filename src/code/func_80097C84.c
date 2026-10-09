#include "types.h"

extern u8 D_80202598[];
extern u8 D_8020C998[];

typedef struct {
    s16 field_00;
    s16 field_02;
    s32 field_04;
    void *field_08;
    void *field_0C;
    s32 field_10;
} State80097C84;

void func_80097C84(State80097C84 *state) {
    state->field_02 = -1;
    state->field_08 = D_80202598;
    state->field_0C = D_8020C998;
    state->field_00 = 0;
    state->field_04 = 0;
    state->field_10 = 0x313B;
}
