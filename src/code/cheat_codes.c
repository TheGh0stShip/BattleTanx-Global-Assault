/* RODATA_VRAM 0x80074140 */
#include "types.h"

extern u8* D_80121C3C[];
extern s16 D_80121CC0;
extern u16 D_80117F48;
extern u8 D_80125AB0;
extern u8 D_80125AB8[];
extern u8 D_80125AB1;
extern u8 D_80121CE0;
extern u8 D_80121CE1;
extern u8 D_B0514720[]; extern u8 D_B0529426[];
extern u8 D_B0529428[]; extern u8 D_B052B554[];
extern u8 D_B052B558[]; extern u8 D_B05336EF[];
extern u8 D_B05336F0[]; extern u8 D_B053A41D[];
extern u8 D_B053A420[]; extern u8 D_B054415A[];
extern u8 D_B0544160[]; extern u8 D_B0548211[];
extern u8 D_B0548218[]; extern u8 D_B054D3CB[];
extern u8 D_B054D3D0[]; extern u8 D_B054F480[];
extern u8 D_B0564C72[]; extern u8 D_B0564C78[];
extern u8 D_B0580EF1[]; extern u8 D_B0580EF8[];
extern u8 D_B058110F[]; extern u8 D_B0581110[];
extern u8 D_B058124A[]; extern u8 D_B0581250[];
extern u8 D_B058151B[]; extern u8 D_B0581520[];
extern u8 D_B0581837[]; extern u8 D_B0581838[];
extern u8 D_B0581B36[]; extern u8 D_B0581B38[];
extern u8 D_B0581C00[];
extern u8 D_80121CE2;
extern u8 D_80125AB2; extern u8 D_80125AB3; extern u8 D_80125AB4;
extern u8 D_80125AB5; extern u8 D_80125AB6;
extern char D_80121B14[]; extern char D_80121B20[];
extern char D_80121B30[]; extern char D_80121B40[];
extern char D_80121B4C[]; extern char D_80121B60[];
extern char D_80121B70[]; extern char D_80121B80[];
extern char D_80121B90[]; extern char D_80121BA0[];
extern char D_80121BB0[]; extern char D_80121BC0[];
extern char D_80121BCC[]; extern char D_80121BDC[];
extern char D_80121BEC[]; extern char D_80121BFC[];
extern char D_80121C0C[]; extern char D_80121C1C[];
extern char D_80121C2C[];
extern void func_800D520C(void* start, void* end);
extern s32 func_800E944C(u8* code);

char* func_800D0070(u8* code) {
    u16 i;
    u8* p;
    u8* s;
    u16 match;

    for (i = 0; i < 33; i++) {
        s = D_80121C3C[i];
        p = code;
        match = 1;
        while (*p != 0) {
            if (*s == 0) break;
            if (*p != *s) { match = 0; break; }
            p++;
            s++;
        }
        if (match && *p == 0 && *s == 0) {
            switch (i) {
            case 0: D_80121CC0 = 1; return D_80121B14;
            case 1: D_80117F48 = 1 - D_80117F48; return D_80121B20;
            case 2: D_80125AB0 ^= 1; return D_80121B30;
            case 3: D_80125AB1 ^= 1; return D_80121B40;
            case 4: D_80125AB1 ^= 1; D_80125AB0 ^= 1; return D_80121B4C;
            case 5: D_80121CE1 = 1 - D_80121CE1; return D_80121B60;
            case 6: D_80121CE0 = 2; func_800D520C(D_B0514720, D_B0529426); return D_80121B70;
            case 7: D_80121CE0 = 2; func_800D520C(D_B0529428, D_B052B554); return D_80121B70;
            case 8: D_80121CE0 = 2; func_800D520C(D_B052B558, D_B05336EF); return D_80121B70;
            case 9: D_80121CE0 = 2; func_800D520C(D_B05336F0, D_B053A41D); return D_80121B70;
            case 10: D_80121CE0 = 2; func_800D520C(D_B053A420, D_B054415A); return D_80121B70;
            case 11: D_80121CE0 = 2; func_800D520C(D_B0544160, D_B0548211); return D_80121B70;
            case 12: D_80121CE0 = 2; func_800D520C(D_B0548218, D_B054D3CB); return D_80121B70;
            case 13: D_80121CE0 = 2; func_800D520C(D_B054D3D0, D_B054F480); return D_80121B70;
            case 14: D_80121CE0 = 2; func_800D520C(D_B054F480, D_B0564C72); return D_80121B70;
            case 15: D_80121CE0 = 2; func_800D520C(D_B0564C78, D_B0580EF1); return D_80121B70;
            case 16: D_80121CE0 = 2; func_800D520C(D_B0580EF8, D_B058110F); return D_80121B70;
            case 17: D_80121CE0 = 2; func_800D520C(D_B0581250, D_B058151B); return D_80121B70;
            case 18: D_80121CE0 = 2; func_800D520C(D_B0581520, D_B0581837); return D_80121B70;
            case 19: D_80121CE0 = 2; func_800D520C(D_B0581838, D_B0581B36); return D_80121B70;
            case 20: D_80121CE0 = 2; func_800D520C(D_B0581B38, D_B0581C00); return D_80121B70;
            case 21: D_80121CE0 = 2; func_800D520C(D_B0581110, D_B058124A); return D_80121B70;
            case 22: D_80121CE2 ^= 1; return D_80121B80;
            case 23: D_80125AB2 ^= 1; return D_80121B90;
            case 24: D_80125AB3 ^= 1; return D_80121BB0;
            case 25: D_80125AB4 ^= 1; return D_80121BC0;
            case 26: D_80125AB5 ^= 1; return D_80121BCC;
            case 27: D_80125AB6 ^= 1; return D_80121BDC;
            case 28: D_80125AB8[0] ^= 1; return D_80121BEC;
            case 29: D_80125AB8[1] ^= 1; return D_80121BFC;
            case 30: D_80125AB8[2] ^= 1; return D_80121C0C;
            case 31: D_80125AB8[3] ^= 1; return D_80121C1C;
            case 32: D_80125AB8[4] ^= 1; return D_80121C2C;
            default: return 0;
            }
        }
    }
    for (i = 0; i < 10; i++) {
        if (*code == 0) return 0;
    }
    if (func_800E944C(code) != 0) return D_80121BA0;
    return 0;
}
