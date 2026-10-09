#include "types.h"

typedef struct {
    u32 opcode;
    u32 address;
} DisplayCommand;

void func_800B9CFC(DisplayCommand *commands, s32 count, s32 offset) {
    register DisplayCommand *cursor __asm__("$4") = commands;
    s32 index;

    for (index = 0; index < count && cursor->opcode != 0xDF000000;
         index++, cursor++) {
        if (*(u8 *)cursor == 0xFD) {
            cursor->address += offset;
        }
    }
}
