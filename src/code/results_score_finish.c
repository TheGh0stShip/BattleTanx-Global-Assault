extern float D_803A5948; void func_800979F4(int);
extern unsigned short D_803A65F0[]; extern unsigned short D_803A6598[]; extern unsigned char D_803A65FC[];
int func_800CF4E4(void) { D_803A65F0[1] += (unsigned int)D_803A5948; if (D_803A65F0[1] >= D_803A6598[1]) { D_803A65F0[1] = D_803A6598[1]; D_803A65FC[1] = 1; func_800979F4(45); return 1; } return 0; }
