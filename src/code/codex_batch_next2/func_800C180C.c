#include "types.h"

extern s32 D_80117EB4;
extern s8 D_80117EB0;
extern s32 D_80117EB8;
extern s32 D_80117EBC;
extern s32 D_80117EC0;
extern s32 D_80117EC4;
extern s32 D_80117EC8;
extern s32 D_80117ECC;
extern s32 D_80117EE4[];
extern s32 D_80117EF4[];
extern s32 D_80117F04[];
extern s32 D_80117F08;

void func_800C180C(void) {
    u16 i;

    if (D_80117EB4 == 6) {
        D_80117EBC = 1;
        D_80117EC4 = 1;
        D_80117ECC = 1;
        for (i = 0; i < 4; i++) {
            D_80117EE4[i] = 1;
        }
        for (i = 0; i < 4; i++) {
            D_80117F04[i] = 2;
        }
        if (D_80117EB0 == 1) {
            D_80117F08 = 1;
        }
    } else {
        D_80117EBC = D_80117EB8;
        D_80117EC4 = D_80117EC0;
        D_80117ECC = D_80117EC8;
        for (i = 0; i < 4; i++) {
            D_80117EE4[i] = D_80117EF4[i];
        }
    }
}
