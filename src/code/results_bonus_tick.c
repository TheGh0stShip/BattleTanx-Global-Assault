extern float D_803A5948; void func_800979F4(int); typedef struct { char pad[0x14]; unsigned short unk14; } Obj;
extern unsigned short D_803A65B8[]; extern unsigned short D_803A6648[]; extern unsigned char D_803A65B4[];
int func_800CF5C8(Obj *a0) {
    unsigned short i = a0->unk14;
    D_803A65B8[i] += (unsigned int)D_803A5948;
    if (D_803A65B8[i] >= D_803A6648[i]) {
        D_803A65B8[i] = D_803A6648[i];
        D_803A65B4[i] = 1;
        func_800979F4(45);
        return 1;
    }
    return 0;
}
