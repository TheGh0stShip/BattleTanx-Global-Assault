typedef struct { int p; short n; } E;
extern signed char D_80117EB0;
extern E D_8011DDF8, D_8011DE00, D_8011DE08, D_8011DE10;
extern char *D_8011DE58, *D_8011DE5C, *D_8011DE60, *D_8011DE64;
extern char D_80117468[], D_8011736C[], D_80117430[], D_80117414[];
void func_800C8F00(int p, unsigned short i, unsigned int t) {
    E *e;
    char **q;
    if (i < D_80117EB0) {
        switch (i) {
        case 1: e = &D_8011DE00; q = &D_8011DE5C; break;
        case 2: e = &D_8011DE08; q = &D_8011DE60; break;
        case 3: e = &D_8011DE10; q = &D_8011DE64; break;
        case 0: default: e = &D_8011DDF8; q = &D_8011DE58; break;
        }
        e->p = p; e->n = 50;
        switch (t) {
        case 10: *q = D_80117468; break;
        case 9: *q = D_8011736C; break;
        case 8: *q = D_80117430; break;
        case 0: case 1: case 2: case 3: case 4: case 5: case 6: case 7: *q = D_80117414; break;
        default: *q = 0; break;
        }
    }
}
