extern int D_8011E93C, D_8011E944;
extern char D_8011EA78[];
extern void func_800BEBA8(void *, int, int);
void func_800CAA0C(int a, int b) {
    D_8011E93C = a;
    D_8011E944 = b;
    func_800BEBA8(D_8011EA78, 0, 1);
}
