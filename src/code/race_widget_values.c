extern signed char D_80117EB0;
extern int D_802195CC;
extern int D_8011EC04, D_8011EC0C, D_8011EC14, D_8011EC1C, D_8011EC24, D_8011EC2C, D_8011EC34, D_8011EC3C;
extern char *func_800CABC0(unsigned short, void *);
extern int func_8009D144(void);
void func_800CAD9C(int val, unsigned short idx) {
    int tmp;
    char *s;
    if (idx < D_80117EB0 && D_802195CC == 3) {
        s = *(char **)(func_800CABC0(idx, &tmp) + 4);
        if (func_8009D144()) {
            *(int *)(s + 0x38) = val;
        } else {
            *(int *)(s + 0x18) = val;
        }
    }
}
void func_800CAE10(int a, int b, unsigned short idx) {
    if (D_802195CC == 3 && idx < D_80117EB0) {
        switch (idx) {
        case 0: D_8011EC04 = a; D_8011EC0C = b; break;
        case 1: D_8011EC14 = a; D_8011EC1C = b; break;
        case 2: D_8011EC24 = a; D_8011EC2C = b; break;
        case 3: D_8011EC34 = a; D_8011EC3C = b; break;
        }
    }
}
