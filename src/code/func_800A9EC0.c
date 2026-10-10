#include "types.h"

typedef struct {
    u8 active;
    u8 pad[0x10B];
} PlayerState;

extern u8 *D_80219498;
extern PlayerState D_80236B33[];

/* Clears each active player's per-player state byte. */
void func_800A9EC0(void) {
    s32 i;

    for (i = 0; i < D_80219498[0xC4]; i++) {
        D_80236B33[i].active = 0;
    }
}
