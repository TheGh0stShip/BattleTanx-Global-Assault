extern short D_803A65D8;
extern short D_803A65C0;
extern short D_803A65B0;
extern unsigned short D_803A663E;
extern char D_80117858[];
void func_8007BDFC(int *, int);
void func_8007C9B8(int *, void *, int, int, float, float);
void func_800979F4(int);

void func_800CFBD8(int *a0, int a1, short *a2) {
    unsigned short count = 0;
    short n = D_803A65D8;
    short x = a2[1];
    short y = a2[2];
    int buf = *a0;

    func_8007BDFC(&buf, 1);
    while (n >= 25) {
        func_8007C9B8(&buf, D_80117858, x, y, 1.0f, 1.0f);
        x += D_803A65C0;
        if (x >= 261) {
            x = a2[1];
            y += D_803A65B0;
        }
        n -= 25;
        count++;
    }
    *a0 = buf;
    if (D_803A663E != count) {
        D_803A663E = count;
        func_800979F4(45);
    }
}
