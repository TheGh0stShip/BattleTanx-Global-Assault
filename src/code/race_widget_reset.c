extern signed char D_80117EB0;
extern int D_8011EC0C, D_8011EC1C, D_8011EC2C, D_8011EC3C;
extern short D_8011F1F8[];
extern char *func_800CABC0(unsigned short, void *);
extern int func_8009D144(void);
extern void func_800BEBA8(void *, int, int);
void func_800CAFA8(unsigned short idx) {
    int tmp[2];
    unsigned char *p;
    char *s;
    char *r;
    if (idx < D_80117EB0) {
        s = *(char **)(func_800CABC0(idx, tmp) + 4);
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
        r = func_800CABC0(idx, &p);
        if (*p) {
            *p = 0;
            D_8011F1F8[idx] = 1;
            func_800BEBA8(r, 0, 1);
        }
    }
}
