extern unsigned int D_803A7040;
extern unsigned char D_80123B04[];
unsigned int func_8009D914(void);
int func_800D8940(void){
 int i = 0;
 int r = func_8009D914() % D_803A7040;
 while (r >= D_80123B04[i]) { r -= D_80123B04[i++]; }
 if (i == 5) { D_80123B04[5]--; D_803A7040--; }
 return i;
}
