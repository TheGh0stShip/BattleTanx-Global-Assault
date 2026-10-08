extern unsigned short D_8011DC80;
extern short D_803A5970;
extern void func_800BF000(void);
int func_800C674C(void) {
    if (D_8011DC80 != 0) return 0;
    D_803A5970 = 1;
    func_800BF000();
    return 1;
}
