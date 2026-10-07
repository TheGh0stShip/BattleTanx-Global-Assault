#include "types.h"

extern u32 func_80077C40(void);
extern void func_80077C78(u32 status);
extern void func_80078048(u32 destination, u8* source, u32 length);
extern u32 func_80078830(u32 command, u32 arg1, u32 arg2, u32 arg3);
extern u32 func_800788E8(u8* string);

u32 func_80078908(void) {
    u32 status = func_80077C40();
    u32 result = func_80078830(0x101, 0, 0, 0);

    func_80077C78(status);
    return result;
}

s32 func_80078954(u8* source, u32 argument) {
    u32 status = func_80077C40();
    u32 length = func_800788E8(source);
    s32 result;

    func_80078048(0xB1FF0000, source, length);
    result = func_80078830(0x202, length, argument, 0);
    if (result & 0x8000) {
        result |= 0xFFFF0000;
    }
    func_80077C78(status);
    return result;
}

s32 func_800789E4(u8* source, u32 argument) {
    u32 status = func_80077C40();
    u32 length = func_800788E8(source);
    s32 result;

    func_80078048(0xB1FF0000, source, length);
    result = func_80078830(0x303, length, argument, 0);
    if (result & 0x8000) {
        result |= 0xFFFF0000;
    }
    func_80077C78(status);
    return result;
}

s32 func_80078A74(u32 argument) {
    u32 status = func_80077C40();
    s32 result = func_80078830(0x404, argument, 0, 0);

    if (result & 0x8000) {
        result |= 0xFFFF0000;
    }
    func_80077C78(status);
    return result;
}
