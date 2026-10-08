extern void *D_8011DC50;
extern short D_8011DC56;
void func_800C2B48(void *);
int func_800BD880(void *);
void func_800C2528(void *, int);
void func_800BEBA8(void *, void *, int);
int func_800C42E4(void *a) {
    func_800C2B48(a);
    func_800C2528(D_8011DC50, func_800BD880(D_8011DC50));
    D_8011DC56 = 0;
    func_800BEBA8(D_8011DC50, a, 1);
    return 1;
}
