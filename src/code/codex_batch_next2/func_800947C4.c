#include "types.h"

typedef struct {
    s32 display_index;
    u8 pad04[0xCC];
} Entry947C4;

extern Entry947C4 D_80122E94[];
extern void *D_803A53A0[];
extern u8 D_1000348[];
extern void func_800AE184(void *display, s32 arg1, s32 arg2, s32 arg3,
                         s32 arg4, s32 arg5, u32 *command, s32 arg7,
                         void *texture, s32 arg9);

void func_800947C4(s32 index, s32 arg1, u8 red, u8 green, u8 blue,
                   s32 shift) {
    u32 command[2];

    command[0] = 0xFA000000;
    command[1] = ((u32)red << 24) | ((u32)green << 16) |
                 ((u32)blue << 8) | 0x80;
    func_800AE184(D_803A53A0[D_80122E94[index].display_index], 0, arg1, 3,
                    0, (1 << shift) & 0xFF, command, 1, D_1000348, 0);
}
