extern float D_803A5948; void func_800979F4(int);
extern unsigned int D_803A65E4[]; extern unsigned int D_803A658C[]; extern unsigned char D_803A6638[];
int func_800CF32C(void) {
    D_803A65E4[0] += (unsigned int)(D_803A5948 * 200.0f);
    if (D_803A65E4[0] >= D_803A658C[0]) {
        D_803A65E4[0] = D_803A658C[0];
        D_803A6638[1] = 1;
        func_800979F4(45);
        return 1;
    }
    return 0;
}
