#include "types.h"

extern u8 D_80236B20[];
extern void _bzero(void* address, s32 size);

void func_800A9D50(void) {
    _bzero(D_80236B20, 0x658);
}
