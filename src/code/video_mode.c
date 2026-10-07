#include "types.h"

typedef struct {
    u32 w0;
    u32 w1;
} GfxCommand;

extern u8 D_80114520[];
extern u8 D_80114558[];
extern u8 D_801145B0[];
extern u8 D_80114610[];
extern void osViSetSpecialFeatures(u32);

void func_8007BDFC(GfxCommand** commands, u16 mode) {
    GfxCommand* command = *commands;

    osViSetSpecialFeatures(0x82);
    switch (mode) {
        case 3: {
            GfxCommand* next = command++;
            next->w0 = 0xDE000000;
            next->w1 = (u32)D_80114520;
            break;
        }
        case 2: {
            GfxCommand* next = command++;
            next->w0 = 0xDE000000;
            next->w1 = (u32)D_801145B0;
            break;
        }
        case 1: {
            GfxCommand* next = command++;
            next->w0 = 0xDE000000;
            next->w1 = (u32)D_80114558;
            break;
        }
        case 4: {
            GfxCommand* next = command++;
            next->w0 = 0xDE000000;
            next->w1 = (u32)D_80114610;
            break;
        }
    }
    *commands = command;
}
