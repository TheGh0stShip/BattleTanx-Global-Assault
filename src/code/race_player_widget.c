extern signed char D_80117EB0;
extern unsigned char D_8011F20A, D_8011F20B, D_8011F20C, D_8011F20D;
extern char D_8011ED48[], D_8011EE18[], D_8011EEB8[], D_8011EFF8[], D_8011EF58[], D_8011F098[], D_8011F138[], D_8011F1D8[];
extern int func_8009D144(void);
char *func_800CABC0(unsigned short idx, unsigned char **out) {
    if (func_8009D144()) {
        if (idx == 0) {
            *out = &D_8011F20A;
            return D_8011ED48;
        } else {
            *out = &D_8011F20B;
            return D_8011EE18;
        }
    }
    switch (idx) {
    case 0:
        *out = &D_8011F20A;
        switch (D_80117EB0) {
        case 1: case 2: case 3: return D_8011EEB8;
        case 4: return D_8011EFF8;
        }
        break;
    case 1:
        switch (D_80117EB0) {
        case 2: *out = &D_8011F20B; return D_8011EF58;
        case 3: *out = &D_8011F20C; return D_8011F138;
        case 4: *out = &D_8011F20B; return D_8011F098;
        }
        break;
    case 2:
        switch (D_80117EB0) {
        case 3: *out = &D_8011F20D; return D_8011F1D8;
        case 4: *out = &D_8011F20C; return D_8011F138;
        }
        break;
    case 3:
        *out = &D_8011F20D; return D_8011F1D8;
    }
    return 0;
}
