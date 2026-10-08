typedef struct { int a, b, c, d; } S;
extern signed char D_80117EB0;
extern S D_803A5FF0[];
void func_800C962C(int a, int b, unsigned short i) {
    if (i < D_80117EB0) { D_803A5FF0[i].a = a; D_803A5FF0[i].b = b; D_803A5FF0[i].c = -1; D_803A5FF0[i].d = -1; }
}
