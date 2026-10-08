extern float D_803A5948; void func_800979F4(int); typedef struct { char pad[0x14]; unsigned short unk14; } Obj;
extern unsigned short D_803A65F0[]; extern unsigned short D_803A6598[]; extern unsigned char D_803A65FC[];
int func_800CF41C(Obj *a0) {
    unsigned short i = a0->unk14;
    D_803A65F0[i] += (unsigned int)D_803A5948;
    if (D_803A65F0[i] >= D_803A6598[i]) {
        D_803A65F0[i] = D_803A6598[i];
        D_803A65FC[i] = 1;
        func_800979F4(45);
        return 1;
    }
    return 0;
}
