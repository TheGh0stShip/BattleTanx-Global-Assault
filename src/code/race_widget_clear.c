extern signed char D_80117EB0;
extern int D_8011EC0C, D_8011EC1C, D_8011EC2C, D_8011EC3C;
extern char *func_800CABC0(unsigned short, void *);
extern int func_8009D144(void);
void func_800CAED0(unsigned short idx) {
    int tmp;
    char *s;
    if (idx < D_80117EB0) {
        s = *(char **)(func_800CABC0(idx, &tmp) + 4);
        if (func_8009D144()) {
            *(int *)(s + 0x38) = 0;
        } else {
            *(int *)(s + 0x18) = 0;
        }
        switch (idx) {
        case 0: D_8011EC0C = 0; break;
        case 1: D_8011EC1C = 0; break;
        case 2: D_8011EC2C = 0; break;
        case 3: D_8011EC3C = 0; break;
        }
    }
}
