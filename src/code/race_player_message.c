typedef struct { int p; short n; } E;
extern signed char D_80117EB0;
extern E D_8011DE38, D_8011DE40, D_8011DE48, D_8011DE50;
void func_800C95A0(int p, unsigned short i) {
    E *e;
    if (i < D_80117EB0) {
        switch (i) {
        case 1: e = &D_8011DE40; break;
        case 2: e = &D_8011DE48; break;
        case 3: e = &D_8011DE50; break;
        case 0: default: e = &D_8011DE38; break;
        }
        e->p = p; e->n = 17;
    }
}
