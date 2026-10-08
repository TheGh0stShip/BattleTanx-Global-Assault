typedef struct { char pad[20]; unsigned short i; } T;
extern float D_803A5948;
extern unsigned int D_803A65E0[], D_803A6588[];
extern char D_803A6638[];
extern void func_800979F4(int);
int func_800CF258(T *a0) {
    unsigned short i = a0->i;
    D_803A65E0[i] += (unsigned int)(D_803A5948 * 200.0f);
    if (D_803A65E0[i] >= D_803A6588[i]) {
        D_803A65E0[i] = D_803A6588[i];
        D_803A6638[i] = 1;
        func_800979F4(45);
        return 1;
    }
    return 0;
}
