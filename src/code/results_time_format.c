extern char D_800740E0[], D_800740EC[];
extern int sprintf(char *, const char *, ...);
extern void func_800BF798(void);
extern void func_800BEBA8(void *, int, int);
extern int D_801177D8, D_801177F4, D_80117810, D_80116EC4, D_80117864, D_80117848, D_8011782C;
extern int D_803A65E0, D_803A6588, D_803A658C, D_803A65A0, D_803A6628, D_803A6640;
extern char D_803A6638, D_803A65FC, D_803A65B4, D_803A65D4, D_803A65C2, D_803A65F8;
extern short D_803A65F0, D_803A6598, D_803A65B8, D_803A6648, D_803A65C4, D_803A6580, D_803A663C, D_803A65D8;
extern short D_803A65C0, D_803A65B0;
extern char D_803A65C8[], D_803A6600[];
extern char D_80120184[];
extern char D_80120660[], D_80121030[];

void func_800CDD70(int a0, int a1, short a2, short a3, int t0, int t1,
                   unsigned short u0, unsigned short u1, unsigned short u2) {
    int h, m, s;
    int h2, m2, s2, r;
    unsigned short v;

    func_800BF798();
    h = t0 / 108000;
    t0 -= h * 108000;
    D_801177D8 = 0;
    D_801177F4 = 0;
    D_80117810 = 0;
    D_80116EC4 = 0;
    D_80117864 = 0;
    D_80117848 = 0;
    D_8011782C = 0;
    D_803A65E0 = a0;
    m = t0 / 1800;
    t0 -= m * 1800;
    D_803A6588 = a1;
    D_803A658C = 0;
    D_803A6638 = 0;
    D_803A65F0 = 0;
    D_803A6598 = a2;
    D_803A65FC = 0;
    D_803A65B8 = 0;
    D_803A6648 = a3;
    D_803A65B4 = 0;
    s = t0 / 30;
    if (h == 0) sprintf(D_803A65C8, D_800740E0, m, s);
    else sprintf(D_803A65C8, D_800740EC, h, m, s);

    h2 = t1 / 108000;
    r = t1 - h2 * 108000;
    m2 = r / 1800;
    r -= m2 * 1800;
    D_803A65D4 = 0;
    D_803A65A0 = 0;
    D_803A6628 = t1;
    s2 = r / 30;
    if (h2 == 0) sprintf(D_803A6600, D_800740E0, m2, s2);
    else sprintf(D_803A6600, D_800740EC, h2, m2, s2);
    D_803A6640 = t1 / 120;
    if (D_803A6640 < 100) D_803A6640 = 100;

    D_803A65C4 = u0;
    D_803A6580 = u1;
    D_803A663C = u2;
    D_803A65C2 = 0;
    D_803A65F8 = 0;
    D_803A65D8 = 0;
    v = u0;
    if ((short)u0 < (short)u2) v = u2;
    if (v <= 500) { D_803A65C0 = 25; D_803A65B0 = 20; }
    else if (v <= 750) { D_803A65C0 = 25; D_803A65B0 = 13; }
    else if (v <= 1000) { D_803A65C0 = 12; D_803A65B0 = 20; }
    else { D_803A65C0 = 12; D_803A65B0 = 13; }
    D_80120184[0] = 'Y';
    D_80120184[1] = 'o';
    D_80120184[2] = 'u';
    D_80120184[3] = 'r';
    func_800BEBA8(D_80120660, 0, 1);
    func_800BEBA8(D_80121030, 0, 1);
}
