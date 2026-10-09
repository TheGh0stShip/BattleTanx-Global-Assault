#include "types.h"

extern void func_80085C30(void *state);

void func_8008616C(void *state) {
    *(s32 *)((u8 *)state + 0x168) = 25;
    func_80085C30(state);
}
