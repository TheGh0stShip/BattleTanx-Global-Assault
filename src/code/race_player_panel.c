extern signed char D_80117EB0;
extern char D_8011E0E4[], D_8011E1E4[], D_8011E384[], D_8011E454[], D_8011E524[], D_8011E2B4[], D_8011DFE4[];
char *func_800C9130(unsigned short a0, short *a1) {
    char *r;
    switch (D_80117EB0) {
    case 2:
        r = D_8011E0E4;
        *a1 = 0;
        if (a0 != 0) {
            if (a0 == 1) return D_8011E1E4;
        }
        return r;
    case 3:
        *a1 = 1;
        switch (a0) {
        case 1: goto l454;
        case 0: default: goto l384;
        case 2: goto l524;
        }
    case 4:
        *a1 = 1;
        switch (a0) {
        case 1: goto l384;
        case 0: default: goto l2b4;
        case 2: goto l454;
        case 3: goto l524;
        }
    case 1: default:
        goto ldef;
    }
l384: return D_8011E384;
l454: return D_8011E454;
l524: return D_8011E524;
l2b4: return D_8011E2B4;
ldef:
    *a1 = 0;
    return D_8011DFE4;
}
