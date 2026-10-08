typedef struct { char pad[8]; void *p; } S;
extern char D_8011DA28[], D_8011DA34[], D_8011DA1C[], D_8011DA40[], D_8011DA4C[], D_8011DA58[], D_8011DA64[];
void func_800C4B24(S *a, unsigned int b) {
    switch (b) {
    case 1: a->p = D_8011DA28; break;
    case 2: a->p = D_8011DA34; break;
    case 3: a->p = D_8011DA1C; break;
    case 4: a->p = D_8011DA40; break;
    case 6: a->p = D_8011DA4C; break;
    case 5: a->p = D_8011DA58; break;
    case 0:
    default: a->p = D_8011DA64; break;
    }
}
