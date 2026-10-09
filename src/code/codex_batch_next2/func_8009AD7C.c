#include "types.h"

extern s32 D_80117F44;
extern s32 D_8021949C;
extern s32 D_802194A0;
extern u8 D_802194A4;
extern u8 D_802194A5;
extern u8 D_802194A6;
extern u8 D_802194A7;
extern u8 D_802194A8;
extern u8 D_802194A9;
extern u8 D_802194AA;
extern u8 D_802194AB;
extern s32 D_802194B0;

extern void func_80097B44(s32 arg0);
extern void func_800A22CC(void);
extern void func_800A87BC(void);
extern void func_800AF43C(void);
extern void func_800A7D70(void);
extern void func_800D4DE0(void);

void func_8009AD7C(void) {
    D_802194A4 = 1;
    D_802194A5 = 1;
    D_802194A6 = 4;
    D_802194A7 = 4;
    D_802194A8 = 0;
    D_802194A9 = 1;
    D_802194AA = 2;
    D_802194AB = 3;
    D_802194A0 = 0;
    D_802194B0 = 0x4B0;
    D_8021949C = D_80117F44;
    func_80097B44(0);
    func_800A22CC();
    func_800A87BC();
    func_800AF43C();
    func_800A7D70();
    func_800D4DE0();
}
