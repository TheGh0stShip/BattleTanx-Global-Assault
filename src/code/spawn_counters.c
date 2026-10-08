typedef struct { short x, y; } P;
extern unsigned char D_80121CD1;
extern P D_803A6F88[];
unsigned short func_800D1234(short x, short y) {
    unsigned char i = D_80121CD1++;
    D_803A6F88[i].x = x;
    D_803A6F88[i].y = y;
    return D_80121CD1 - 1;
}
void func_800D1274(unsigned short i, float *x, float *y) {
    *x = D_803A6F88[i].x;
    *y = D_803A6F88[i].y;
}
