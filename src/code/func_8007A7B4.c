#include "types.h"

typedef struct {
    u8 pad[0xB2];
    s16 slot2;
    s16 slot1;
    s16 slot0;
} SlotState;

extern SlotState *D_80114500;

s32 func_8007A7B4(void) {
    s16 index;

    for (index = 0; index < 3; index++) {
        if (index != D_80114500->slot0 &&
            index != D_80114500->slot1 &&
            index != D_80114500->slot2) {
            return index;
        }
    }
    return -1;
}
