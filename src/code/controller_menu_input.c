typedef struct { int a; int b; unsigned short c; char ext[4]; unsigned char name[16]; char pad[2]; } Save;
extern Save D_803A6360[16];
extern char D_80116EB8[], D_80116E80[];
extern void *D_8011F82C, *D_8011F830, *D_8011F834, *D_8011F83C, *D_8011F844, *D_8011F84C;
extern char D_8011F4A0[], D_8011F4D8[], D_8011F2A8[], D_8011F4F8[], D_8011F434[], D_8011F3FC[];
extern char D_8011F55C[], D_8011F470[], D_8011F3E0[], D_8011F3D8[], D_8011F900[], D_8011F5F4[];
extern char D_8011F644[], D_8011F61C[], D_8011FA40[], D_8011F51C[], D_8011F2C4[], D_8011F2F0[];
extern char D_8011F37C[], D_8011F2E8[], D_803A6160[], D_803A62D4[], D_8011F5BC[], D_8011F5DC[], D_8011F650[];
extern char D_8007403C[];
extern unsigned char D_8011F23B;
extern unsigned short D_8011F236, D_8011F238, D_8011F828, D_8011481C, D_803A6350;
extern int D_80114820, D_803A62A8, D_803A6358;
extern void func_800BEBA8(void *, int, int);
extern unsigned short func_800CC2F8(int);
extern unsigned short func_800CC0B0(void);
extern int sprintf(char *, const char *, ...);

unsigned short func_800CC574(int mode, unsigned short sub, int arg) {
    unsigned short r;
    unsigned short i;
    unsigned short j;
    unsigned short count;
    int idx;

    D_8011F82C = D_80116EB8;
    D_8011F830 = 0;
    D_8011F84C = 0;
    switch (mode) {
    case 6:
        switch (sub) {
        case 2:
            D_8011F834 = D_8011F4A0;
            break;
        case 5:
        case 10:
            D_8011F834 = D_8011F4D8;
            D_8011F83C = D_8011F2A8;
            D_8011F84C = D_8011F4F8;
            D_8011F830 = D_80116E80;
            goto tail6;
        case 1:
            if (D_8011F23B) {
                D_8011F834 = D_8011F434;
                D_8011F23B = 0;
            } else {
                D_8011F834 = D_8011F3FC;
                D_8011F23B = 1;
            }
            break;
        case 0xFFFF:
            D_8011F834 = D_8011F55C;
            break;
        default:
            D_8011F834 = D_8011F470;
            break;
        }
        D_8011F83C = D_8011F2A8;
    tail6:
        if (D_803A62A8 == 1) {
            D_8011F844 = D_8011F3E0;
        } else {
            D_8011F844 = D_8011F3D8;
        }
        func_800BEBA8(D_8011F900, arg, 0);
        D_8011F236 = 0;
        break;
    case 8:
        D_8011F834 = D_8011F5F4;
        D_8011F82C = 0;
        D_8011F844 = 0;
        D_8011F83C = D_8011F644;
        func_800BEBA8(D_8011F900, arg, 0);
        break;
    case 9:
        D_8011F834 = D_8011F61C;
        D_8011F82C = 0;
        D_8011F844 = 0;
        D_8011F83C = D_8011F3D8;
        func_800BEBA8(D_8011F900, arg, 0);
        break;
    case 1:
        D_8011F238 = 0;
        r = func_800CC2F8(0);
        if (r != 0) return r;
        D_8011F834 = D_8011F51C;
        D_8011F83C = D_8011F2C4;
        D_8011F844 = D_8011F3D8;
        func_800BEBA8(D_8011FA40, arg, 0);
        break;
    case 3:
        D_8011F238 = 0;
        r = func_800CC2F8(0);
        if (r != 0) return r;
        count = 0;
        for (i = 0; i < 16; i++) {
            if (D_803A6360[i].name[0] == 0) count++;
        }
        if (D_8011F236 == 0 && func_800CC0B0() != 0) {
            idx = -1;
        } else {
            for (j = 0; j < 16; j++) {
                if (D_803A6360[j].c == D_8011481C && D_803A6360[j].b == D_80114820) {
                    idx = j;
                    goto found;
                }
            }
            idx = -1;
        found:;
        }
        if (D_8011F828 == 0 || (count == 0 && idx < 0)) {
            D_8011F834 = D_8011F2F0;
        } else {
            D_8011F834 = D_8011F37C;
        }
        D_8011F83C = D_8011F2E8;
        D_8011F844 = D_8011F3D8;
        func_800BEBA8(D_8011FA40, arg, 0);
        break;
    case 4:
        sprintf(D_803A6160, D_8007403C, D_803A62D4);
        D_8011F83C = D_8011F2E8;
        D_8011F834 = D_803A6160;
        D_8011F844 = D_8011F3D8;
        func_800BEBA8(D_8011FA40, arg, 0);
        break;
    case 7:
        D_8011F834 = D_8011F5BC;
        D_8011F82C = 0;
        D_8011F844 = 0;
        D_8011F83C = D_8011F5DC;
        func_800BEBA8(D_8011F900, arg, 0);
        break;
    case 10:
        D_8011F834 = D_8011F650;
        D_8011F83C = D_8011F644;
        D_8011F844 = D_8011F3D8;
        func_800BEBA8(D_8011F900, arg, 0);
        break;
    }
    D_803A6358 = mode;
    D_803A6350 = sub;
    return 0;
}
