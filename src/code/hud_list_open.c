extern unsigned short D_8011DC74;
extern char D_8011C33C[], D_8011CD88[], D_8011CEF8[], D_8011D068[], D_8011D1C8[], D_8011C9A4[];
void func_800BEBA8(void *, int, int);
void func_800BF130(void *);
void func_800C47D4(void) {
    if (D_8011DC74 != 0) {
        func_800BEBA8(D_8011C33C, 0, 0);
        func_800BF130(D_8011CD88);
        func_800BF130(D_8011CEF8);
        func_800BF130(D_8011D068);
        func_800BF130(D_8011D1C8);
        func_800BF130(D_8011C9A4);
    }
}
