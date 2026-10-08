extern unsigned short D_8011DC80;
extern char D_8011D3EC[], D_8011DAE4[], D_8011DB54[], D_8011DBC4[], D_8011DC34[], D_8011D9CC[];
extern void func_800BEBA8(void *, int, int);
extern void func_800BF130(void *);
void func_800C67BC(void) {
    if (D_8011DC80 != 0) {
        func_800BEBA8(D_8011D3EC, 0, 0);
        func_800BF130(D_8011DAE4);
        func_800BF130(D_8011DB54);
        func_800BF130(D_8011DBC4);
        func_800BF130(D_8011DC34);
        func_800BF130(D_8011D9CC);
    }
}
