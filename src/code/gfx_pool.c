#include "types.h"

extern s32 D_80168080;
extern s32 D_80168084;
extern s32 D_80168088;
extern s32 D_8016808C;
extern u8 D_801777E0[];
extern s16 D_80114510;
extern u8 D_80114512;

extern void _bzero(void* address, s32 size);
extern void func_800A9F10(void);

s32 func_8007B030(void) {
    return 0x44E50;
}

void func_8007B03C(void) {
    D_80168080 = 0;
    D_80168084 = 0;
    D_80168088 = 0;
    D_8016808C = 0;
    _bzero(D_801777E0, 0x300);
    _bzero(D_801777E0 + 0x5680, 0x80);
    D_80114510 = 0;
    D_80114512 = (D_80114512 + 1) % 3;
    func_800A9F10();
}
