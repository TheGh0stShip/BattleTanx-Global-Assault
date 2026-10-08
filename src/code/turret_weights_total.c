extern int D_803A7040;
extern unsigned char D_80123B04[];
extern unsigned char D_80123AB0[];
unsigned int func_8009D914(void);
void func_800D88F0(void){
 int i;
 D_803A7040 = 0;
 D_80123B04[5] = 1;
 for (i = 0; i < 20; i++) D_803A7040 += D_80123B04[i];
}
