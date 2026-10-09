#include "types.h"

extern void func_800B22F8(u16 id);

void func_80082560(void *object) {
    u16 id = *(u16 *)((u8 *)object + 0x134);

    if (id != 0xFFFF) {
        func_800B22F8(id);
        *(u16 *)((u8 *)object + 0x134) = 0xFFFF;
    }
}
