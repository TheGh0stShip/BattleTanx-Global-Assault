extern char D_803A6650[];
extern char D_803A61A4[];
extern unsigned int D_80117EB8;
extern unsigned char D_80117EC3, D_80117ECB, D_80117ED0, D_80117ED1;
extern void func_800E96B4(int, int, int, int, void *);
extern char *func_800CB334(char *);
void func_800CD85C(int a, int b, int c, int d) {
    char *p;
    func_800E96B4(a, b, c, d, D_803A6650);
    p = D_803A61A4;
    *p++ = a;
    switch (D_80117EB8) {
    case 1: *p++ = 0; break;
    case 2: *p++ = 1; break;
    case 3: *p++ = 2; break;
    }
    p += 2;
    *(int *)p = b;
    p += 4;
    ((int *)p)[0] = c;
    ((int *)p)[1] = d;
    p = func_800CB334(p);
    *p++ = D_80117EC3;
    *p++ = D_80117ECB;
    *p++ = D_80117ED0;
    *p++ = D_80117ED1;
    *p = 0;
}
