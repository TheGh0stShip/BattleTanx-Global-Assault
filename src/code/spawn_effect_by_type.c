extern char D_80115444[], D_801150A4[], D_8011551C[], D_80115868[], D_801159F4[], D_80115834[];
extern char D_80114EB0[], D_80114EC8[], D_80114EE0[], D_80114EF8[];
extern int func_800EC1F8(void *, int, int, int, int, int);
extern void func_800A2C6C(int, void *, int, int, int, int);
extern void func_800A5BD8(void *, int, int, float, void *, int);
void func_800D0DF8(unsigned char type, void *obj) {
    int extra = 0;
    void *p;
    switch (type) {
    case 0:
        extra = func_800EC1F8(obj, 0, 0, 100, 2, 0);
        p = D_80115444;
        break;
    case 1:
        func_800A2C6C(0, obj, 172, 50, 0, 0xE49D0A);
        p = D_801150A4;
        break;
    case 2: p = D_8011551C; break;
    case 3: p = D_80115868; break;
    case 4: p = D_801159F4; break;
    case 5: p = D_80115834; break;
    case 7: p = D_80114EB0; break;
    case 8: p = D_80114EC8; break;
    case 9: p = D_80114EE0; break;
    case 10: p = D_80114EF8; break;
    default: p = 0; break;
    }
    if (p != 0) {
        func_800A5BD8(obj, 0, 0, 1.0f, p, extra);
    }
}
