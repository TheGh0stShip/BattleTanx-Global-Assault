/* SPAN 0x80078C68 */
#include "types.h"

extern u32 func_80077C40(void);
extern void func_80077C78(u32 status);
extern void func_80077DA4(u32 source, u8* destination, u32 length);
extern void func_80078048(u32 destination, u8* source, u32 length);
extern u32 func_80078830(u32 command, u32 arg1, u32 arg2, u32 arg3);

s32 func_80078ADC(u32 address, u8* destination, u32 length) {
    s32 total;
    u32 status;
    u32 chunk;
    u32 count;

    status = func_80077C40();
    total = 0;
    while (length != 0) {
        chunk = length;
        if (chunk > 0x8000) {
            chunk = 0x8000;
        }
        count = func_80078830(0x505, address, chunk, 0);
        if (count == 0) {
            func_80077C78(status);
            return total;
        }
        func_80077DA4(0xB1FF0000, destination, count);
        destination += count;
        length -= count;
        total += count;
    }
    func_80077C78(status);
    return total;
}
s32 func_80078BA0(u32 address, u8* source, u32 length) {
    s32 total;
    u32 status;
    u32 chunk;
    u32 count;

    status = func_80077C40();
    total = 0;
    while (length != 0) {
        chunk = length;
        if (chunk > 0x8000) {
            chunk = 0x8000;
        }
        func_80078048(0xB1FF0000, source, chunk);
        count = func_80078830(0x606, address, chunk, 0);
        if (count == 0) {
            func_80077C78(status);
            return total;
        }
        source += count;
        length -= count;
        total += count;
    }
    func_80077C78(status);
    return total;
}
