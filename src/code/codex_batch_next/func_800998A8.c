#include "types.h"

extern u8 D_80219460;
extern u8 D_80219461;
extern u8 D_80219462;
extern u8 D_80219463;

void func_800998A8(s32 index, u8 first, u8 second, u8 third, u8 fourth) {
    s32 offset = index * 4;

    *(u8 *)((u8 *)&D_80219460 + offset) = first;
    *(u8 *)((u8 *)&D_80219461 + offset) = second;
    *(u8 *)((u8 *)&D_80219462 + offset) = third;
    *(u8 *)((u8 *)&D_80219463 + offset) = fourth;
}
