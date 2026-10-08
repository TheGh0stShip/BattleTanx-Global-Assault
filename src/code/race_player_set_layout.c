extern signed char D_80117EB0;
extern char D_8011E0E4[], D_8011E1E4[], D_8011E384[], D_8011E454[], D_8011E524[], D_8011E2B4[], D_8011DFE4[];
static inline char *h9130(unsigned short a0, short *a1) {
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
extern char D_80117468[], D_8011736C[], D_80117334[], D_80117350[], D_801173A4[], D_801173C0[], D_801173DC[], D_801174A0[], D_801174BC[], D_80117484[], D_80117388[], D_8011744C[], D_80117430[], D_801174D8[], D_801174F4[];
static inline char *h9010(int a0) {
    switch (a0) {
    case 2: return D_80117468;
    case 3: return D_8011736C;
    case 4: return D_80117334;
    case 5: case 16: return D_80117350;
    case 6: return D_801173A4;
    case 15: return D_801173C0;
    case 7: return D_801173DC;
    case 8: return D_801174A0;
    case 9: return D_801174BC;
    case 10: return D_80117484;
    case 11: return D_80117388;
    case 14: return D_8011744C;
    case 12: return D_80117430;
    case 13: return D_801174D8;
    case 17: return D_801174F4;
    }
    return 0;
}
typedef struct { int p; short n; } E;
extern E D_8011DE18, D_8011DE20, D_8011DE28, D_8011DE30;
extern char *D_8011DE68, *D_8011DE6C, *D_8011DE70, *D_8011DE74;
void func_800C924C(int p, unsigned short i, unsigned int t) {
    E *e;
    char **q;
    if (i < D_80117EB0) {
        unsigned short flag;
        char *x = *(char **)(h9130(i, (short *)&flag) + 4);
        if (flag) {
            if (t == 0) { x[48] = 1; x[64] = 1; x[80] = 1; }
            else { x[48] = 3; x[64] = 8; x[80] = 14; }
        } else {
            if (t == 0) { x[64] = 1; x[80] = 1; x[96] = 1; x[112] = 1; }
            else { x[64] = 3; x[80] = 10; x[96] = 8; x[112] = 14; }
        }
        switch (i) {
        case 1: e = &D_8011DE20; q = &D_8011DE6C; break;
        case 2: e = &D_8011DE28; q = &D_8011DE70; break;
        case 3: e = &D_8011DE30; q = &D_8011DE74; break;
        case 0: default: e = &D_8011DE18; q = &D_8011DE68; break;
        }
        e->p = p; e->n = 50;
        *q = h9010(t);
    }
}
