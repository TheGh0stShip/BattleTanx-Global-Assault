extern int D_801177D8, D_801177F4, D_80117810, D_80116EC4, D_80117864, D_80117848, D_8011782C;
extern int D_803A65E0, D_803A6588, D_803A65E4, D_803A658C;
extern char D_803A6638, D_803A6639, D_803A65FC, D_803A65FD, D_803A65B4, D_803A65B5;
extern short D_803A65F0, D_803A65F2, D_803A65B8, D_803A65BA;
extern unsigned short D_803A6598, D_803A659A, D_803A6648, D_803A664A;
extern char D_803A65C8[], D_803A6600[], D_800740E0[], D_800740EC[];
extern char D_803A65D4, D_803A65C2, D_803A65F8;
extern int D_803A65A0, D_803A6628, D_803A6640;
extern unsigned short D_803A6580, D_803A65C4, D_803A65D8, D_803A663C;
extern short D_803A65C0, D_803A65B0;
extern char D_80120184, D_80120185, D_80120186, D_80120187;
extern char D_80120760[], D_801211C0[];
void func_800BF798(void);
int func_80110044(char *, const char *, ...);
void func_800BEBA8(void *, int, int);

void func_800CE188(int a0, int a1, int a2, int a3, unsigned short a4, unsigned short a5,
                   unsigned short a6, unsigned short a7, int t1, int t2,
                   unsigned short a10, unsigned short a11, unsigned short a12) {
    int h, m, s, h2, m2, s2;
    unsigned short v;

    func_800BF798();
    D_801177D8 = 0;
    D_801177F4 = 0;
    D_80117810 = 0;
    D_80116EC4 = 0;
    D_80117864 = 0;
    D_80117848 = 0;
    D_8011782C = 0;
    D_803A65E0 = a0;
    D_803A6588 = a1;
    D_803A65E4 = a2;
    D_803A658C = a3;
    D_803A6638 = 0;
    D_803A6639 = 0;
    D_803A65F0 = 0;
    D_803A65F2 = 0;
    D_803A65FC = 0;
    D_803A65FD = 0;
    D_803A65B8 = 0;
    D_803A65BA = 0;
    D_803A65B4 = 0;
    D_803A65B5 = 0;
    D_803A6598 = a4;
    D_803A659A = a5;
    D_803A6648 = a6;
    D_803A664A = a7;
    h = t1 / 108000;
    t1 %= 108000;
    m = t1 / 1800;
    t1 %= 1800;
    s = t1 / 30;
    if (h == 0) {
        func_80110044(D_803A65C8, D_800740E0, m, s);
    } else {
        func_80110044(D_803A65C8, D_800740EC, h, m, s);
    }
    D_803A65D4 = 0;
    D_803A65A0 = 0;
    D_803A6628 = t2;
    h2 = t2 / 108000;
    m2 = (t2 % 108000) / 1800;
    s2 = ((t2 % 108000) % 1800) / 30;
    if (h2 == 0) {
        func_80110044(D_803A6600, D_800740E0, m2, s2);
    } else {
        func_80110044(D_803A6600, D_800740EC, h2, m2, s2);
    }
    D_803A6640 = t2 / 120;
    if (D_803A6640 < 100) {
        D_803A6640 = 100;
    }
    D_803A65C2 = 0;
    D_803A65F8 = 0;
    D_803A6580 = a11;
    D_803A65C4 = a10;
    D_803A65D8 = a10;
    D_803A663C = a12;
    v = a10;
    if ((short)a10 < (short)a12) {
        v = a12;
    }
    if (v <= 500) {
        D_803A65C0 = 25;
        D_803A65B0 = 20;
    } else if (v <= 750) {
        D_803A65C0 = 25;
        D_803A65B0 = 13;
    } else if (v <= 1000) {
        D_803A65C0 = 12;
        D_803A65B0 = 20;
    } else {
        D_803A65C0 = 12;
        D_803A65B0 = 13;
    }
    D_80120184 = 'Y';
    D_80120185 = 'o';
    D_80120186 = 'u';
    D_80120187 = 'r';
    func_800BEBA8(D_80120760, 0, 1);
    func_800BEBA8(D_801211C0, 0, 1);
}
