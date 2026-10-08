extern float D_803A5948; void func_800979F4(int);
extern unsigned short D_803A65B8[]; extern unsigned short D_803A6648[]; extern unsigned char D_803A65B4[];
int func_800CF690(void) { D_803A65B8[1] += (unsigned int)D_803A5948; if (D_803A65B8[1] >= D_803A6648[1]) { D_803A65B8[1] = D_803A6648[1]; D_803A65B4[1] = 1; func_800979F4(45); return 1; } return 0; }
