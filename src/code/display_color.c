#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} DisplayCommand;

void func_800B9A4C(DisplayCommand* command, u8* color) {
    s32 count = 0;
    u32 end_opcode = 0xDF000000;
    u32 color_opcode = 0xFB000000;
    DisplayCommand* current;

    current = command;

    do {
        u32 opcode = current->w0;
        if (opcode == end_opcode) {
            break;
        }
        if (opcode == color_opcode) {
            current->w0 = opcode;
            current->w1 = (color[0] << 24) | (color[1] << 16) |
                          (color[2] << 8) | 0xFF;
        }
        count++;
        current++;
    } while (count < 1000);
}
