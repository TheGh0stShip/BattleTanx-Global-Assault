/* ---- 0x800E9000/c/e92f0.c ---- */
typedef struct { int a, b, c, d; } S;
typedef struct { int a, b; } P;
extern S D_80125548;
extern int D_8012554C;
extern S D_803A8310;
extern unsigned short D_80117F48;
extern int D_80117F44;
extern unsigned char D_80121CE2;
extern P *D_801254C0[];
void func_800E92F0(void) {
    int i, j;
    int *p = &D_8012554C;
    if (*p != -1) {
        D_803A8310 = D_80125548;
        *p = -1;
        return;
    }
    D_803A8310.a = 0;
    if (D_80117F48 != 0) {
        for (i = 0; i < 34; i++) {
            if (D_801254C0[i]->a == 0 && D_801254C0[i]->b == D_80117F44) {
                if (D_80121CE2 == 0 && D_801254C0[i - 1]->a == 1) {
                    for (j = i - 1; j > 0 && D_801254C0[j - 1]->a == 1; j--) {
                    }
                    D_803A8310.a = j;
                } else {
                    D_803A8310.a = i;
                }
            }
        }
    }
    D_803A8310.d = 0;
    D_803A8310.c = 0;
    D_803A8310.b = 30;
}

