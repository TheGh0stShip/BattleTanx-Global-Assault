typedef struct { char pad[8]; void *p; } S;
extern char D_8011DA10[], D_8011DA04[], D_8011D9FC[];
void func_800C4ABC(S *a, unsigned int b) {
    switch (b) {
    case 1: a->p = D_8011DA10; break;
    case 3: a->p = D_8011DA04; break;
    case 2:
    default: a->p = D_8011D9FC; break;
    }
}
