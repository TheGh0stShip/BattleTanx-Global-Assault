#include "types.h"

extern void func_80077CA8(u32* address, u32 value);
extern void func_80077FD0(u32 address, u8 value);

void func_80078048(u32 destination, u8* source, u32 length) {
    while (length != 0 && (destination & 3) != 0) {
        func_80077FD0(destination++, *source++);
        length--;
    }
    while (length >= 4) {
        u32 value = ((u32)source[0] << 24) | ((u32)source[1] << 16) |
                    ((u32)source[2] << 8) | source[3];

        func_80077CA8((u32*)destination, value);
        destination += 4;
        source += 4;
        length -= 4;
    }
    while (length != 0) {
        func_80077FD0(destination++, *source++);
        length--;
    }
}

void func_80078134(u32 destination, u8 value, u32 length) {
    while (length != 0 && (destination & 3) != 0) {
        func_80077FD0(destination++, value);
        length--;
    }
    if (length >= 4) {
        u32 word = (value << 24) | (value << 16) | (value << 8) | value;

        while (length >= 4) {
            func_80077CA8((u32*)destination, word);
            destination += 4;
            length -= 4;
        }
    }
    while (length != 0) {
        func_80077FD0(destination++, value);
        length--;
    }
}
