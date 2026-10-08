extern unsigned int D_802194A0, D_8021949C; extern int D_802194B0;
extern signed char D_80117EB0;
extern char D_801177B0[], D_80117794[], D_80117708[], D_801176EC[], D_80117724[];
extern int D_8011DE78, D_8011DE7C, D_8011DE80, D_8011DE84;
extern void *D_8011DE88, *D_8011DE8C, *D_8011DE90, *D_8011DE94;
void func_800C975C(int v, unsigned short i) {
    int *a;
    void **out;
    void *r;
    if (i < D_80117EB0) {
        switch (i) {
        case 1: a = &D_8011DE7C; out = &D_8011DE8C; break;
        case 2: a = &D_8011DE80; out = &D_8011DE90; break;
        case 3: a = &D_8011DE84; out = &D_8011DE94; break;
        case 0: default: a = &D_8011DE78; out = &D_8011DE88; break;
        }
        *a = v;
        switch (D_802194A0) {
        case 0: case 8: r = D_801177B0; break;
        case 2: r = D_80117724; break;
        case 7:
            if (D_802194B0 == 0) { r = 0; break; }
        case 13: case 14:
            switch (D_8021949C) {
            case 2: r = D_80117794; break;
            case 4: r = D_80117708; break;
            case 12: r = D_801176EC; break;
            case 6: r = D_80117724; break;
            default: r = 0; break;
            }
            break;
        default: r = 0; break;
        }
        *out = r;
    }
}
