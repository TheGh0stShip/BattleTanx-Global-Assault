/* ---- 0x800D2000/seq_misc.c ---- */
extern char *D_803A66B8;
extern char D_803A6678[];
extern unsigned short D_803A701E;
extern int D_80121CDC;
void func_8009F4B4(void *, void *, int);

void func_800D250C(int arg0, unsigned short *arg1, int *arg2) {
    func_8009F4B4(D_803A66B8 + 0x48, D_803A6678, arg0);
    *arg1 = D_803A701E;
    *arg2 = D_80121CDC;
}

extern float D_80074490;
extern float D_80074494;
extern double D_80074498;
extern float D_80236068;
extern char D_01000150[], D_01000168[], D_010001E0[];

void func_800D2574(char *arg0, float arg1) {
    float f = arg1 * D_80074490 + D_80074494;
    if (arg0 == D_01000150 || arg0 == D_01000168 || arg0 == D_010001E0) {
        f = f * D_80074498;
    }
    D_80236068 = f;
}

