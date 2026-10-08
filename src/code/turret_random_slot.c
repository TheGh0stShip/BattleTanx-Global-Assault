extern int D_803A7040;
extern unsigned char D_80123B04[];
extern unsigned char D_80123AB0[];
unsigned int func_8009D914(void);
int func_800D89F8(int a){
 while (D_80123AB0[a] == 0) a = func_8009D914() % 19 + 1;
 return a;
}
