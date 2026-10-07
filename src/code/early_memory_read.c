#include "types.h"

extern u32 func_80077CE0(u32* address);
extern u8 func_80077D64(u32 address);

void func_80077DA4(u32 source, u8* destination, u32 length) {
    while (length != 0 && (source & 3) != 0) {
        *destination++ = func_80077D64(source++);
        length--;
    }
    while (length >= 4) {
        u32 value = func_80077CE0((u32*)source);

        *destination++ = value >> 24;
        *destination++ = value >> 16;
        *destination++ = value >> 8;
        *destination++ = value;
        source += 4;
        length -= 4;
    }
    while (length != 0) {
        *destination++ = func_80077D64(source++);
        length--;
    }
}

void func_80077E80(u32 address, u8 value) {
    u32 shift = ~address;
    u32 mask;

    shift &= 3;
    shift <<= 3;
    address &= ~3;
    mask = 0xFF << shift;
    *(u32*)address = (*(u32*)address & ~mask) |
                     ((value & 0xFF) << shift);
}

void func_80077EBC(u32 source, u32 destination, u32 length) {
    while (length != 0 && (source & 3) != 0) {
        func_80077E80(destination++, func_80077D64(source++));
        length--;
    }
    while (length >= 4) {
        u32 value = func_80077CE0((u32*)source);

        func_80077E80(destination++, value >> 24);
        func_80077E80(destination++, value >> 16);
        func_80077E80(destination++, value >> 8);
        func_80077E80(destination++, value);
        source += 4;
        length -= 4;
    }
    while (length != 0) {
        func_80077E80(destination++, func_80077D64(source++));
        length--;
    }
}
