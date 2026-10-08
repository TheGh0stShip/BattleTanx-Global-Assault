extern char *D_80120764;
extern char *D_801211C4;
extern int D_8012012C, D_8012019C, D_801201A8;
extern int D_8021949C;
extern char D_803A665C;
extern int D_803A8310;
void func_800CD85C(int, int, int, int);
void func_800CE188(int, int, int, int, int, int, int, int, int, int, int, int, int);

void func_800CE814(int a0, int a1, int a2, int a3, int a4, int a5,
                   int a6, int a7, int a8, int a9, int a10, int a11, int a12) {
    char *p = D_80120764;
    *(int **)(p + 184) = &D_801201A8;
    *(int **)(p + 216) = &D_8012019C;
    *(short *)(p + 194) = 185;
    *(short *)(p + 210) = 200;
    *(int **)(p + 136) = &D_8012012C;
    {
    unsigned short b4 = a4, b5 = a5, b6 = a6, b7 = a7; short b10 = a10, b11 = a11, b12 = a12;
    D_803A665C = 1;
    if (D_8021949C == 16) {
        p[176] = 1;
        *(short *)(p + 162) = 175;
    } else {
        p[176] = 4;
        *(short *)(p + 162) = 35;
    }
    p = D_801211C4;
    p[160] = 4;
    p[176] = 5;
    func_800CD85C(D_803A8310 + 1, a1, a3, b12);
    func_800CE188(a0, a1, a2, a3, b4, b5, b6, b7, a8, a9, b10, b11, b12);
    }
}
