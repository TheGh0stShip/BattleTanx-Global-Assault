#include "types.h"

extern u8 D_80236B20;
extern u8 D_80236B21;
extern u8 D_80236B22;
extern u8 D_80236B23;
extern u8 D_80236B24;
extern u8 D_80236B25;
extern u8 D_80236B26;
extern u8 D_80236B27;
extern u8 D_80236B28;
extern u8 D_80236B29;
extern u8 D_80236B2A;
extern u8 D_80236B2B;
extern u8 D_80236B2C;

void func_800A9C24(s32 count, u8 a, u8 b, u8 c, u8 d, u8 e, u8 f,
                   u8 g, u8 h, u8 i, u8 j, u8 k, u8 l) {
    D_80236B20 = count;
    if (count > 0) {
        D_80236B21 = a;
        D_80236B22 = b;
        D_80236B23 = c;
        D_80236B24 = d;
        D_80236B25 = e;
        D_80236B26 = f;
    }
    if (count >= 2) {
        D_80236B27 = g;
        D_80236B28 = h;
        D_80236B29 = i;
        D_80236B2A = j;
        D_80236B2B = k;
        D_80236B2C = l;
    }
}
