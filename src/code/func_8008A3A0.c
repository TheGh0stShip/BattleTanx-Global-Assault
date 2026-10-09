#include "types.h"

extern u8 D_80235F00[];

void func_8008A3A0(void *arg0, s32 arg1) {
    s32 temp;
    s32 index;
    s32 *entry;
    s32 *slot;
    s32 slot_offset;

    temp = arg1 & 0xFFFF;
    index = temp;
    if (temp == 0x7F) {
        entry = 0;
    } else {
        entry = (s32 *)(D_80235F00 + index * 0x250);
    }
    slot_offset = ((arg1 & 0xFFFF) * 8) + 4;
    __asm__("" : "=r"(slot_offset) : "0"(slot_offset));
    slot = (s32 *)((u8 *)arg0 + slot_offset);
    slot[0] = (s32)entry;
    slot[1] = *entry;
}
