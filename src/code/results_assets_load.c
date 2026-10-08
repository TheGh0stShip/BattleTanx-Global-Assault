extern char D_B04729C0[], D_B0472BAC[], D_B0472BB0[], D_B0472D9C[];
typedef struct { int pad[3]; int flag; } DmaReq;
extern DmaReq D_80116860, D_8011687C;
extern int D_80116EA8, D_80116EC4, D_80116E8C, D_80116A64, D_80116A80, D_80116A9C, D_80116AB8, D_80116AD4, D_801172EC, D_80117308;
extern char D_8011F804[];
extern void func_80096810(void *, void *, DmaReq *, int);
extern void func_800BEBA8(void *, int, int);
void func_800CB110(void) {
    D_80116860.flag = 0;
    D_8011687C.flag = 0;
    func_80096810(D_B04729C0, D_B0472BAC, &D_80116860, 0);
    func_80096810(D_B0472BB0, D_B0472D9C, &D_8011687C, 1);
    D_80116EA8 = 0; D_80116EC4 = 0; D_80116E8C = 0;
    D_80116A64 = 0; D_80116A80 = 0; D_80116A9C = 0; D_80116AB8 = 0; D_80116AD4 = 0;
    D_801172EC = 0; D_80117308 = 0;
    func_800BEBA8(D_8011F804, 0, 1);
}
