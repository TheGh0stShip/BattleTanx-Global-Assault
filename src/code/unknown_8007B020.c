#include "types.h"

extern u8 D_80168090[];

/* Base of the 0x44E50-byte arena passed to the game's asset loaders. */
void* func_8007B020(void) {
    return D_80168090;
}
