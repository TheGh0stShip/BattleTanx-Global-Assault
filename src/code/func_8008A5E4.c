#include "types.h"

void func_8008A5E4(void *p) {
    u16 i;

    *((u8 *)p + 0) = 1;
    *((u8 *)p + 1) = 0;
    *(u16 *)((u8 *)p + 2) = 0;
    i = 0;
    do {
        register u32 offset asm("$2") = i * 8;
        s32 *entry = (s32 *)(offset + (u32)p);

        entry = (s32 *)((u8 *)entry + 4);
        i++;
        entry[0] = 0;
        entry[1] = 0;
    } while (i < 5);
}
