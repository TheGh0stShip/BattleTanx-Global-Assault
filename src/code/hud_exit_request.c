extern unsigned short D_8011DC74;
extern short D_803A5970;
void func_800BF000(void);
int func_800C42A0(void) {
    int r;
    if (D_8011DC74 == 0) {
        D_803A5970 = 1;
        func_800BF000();
        r = 1;
    } else {
        r = 0;
    }
    return r;
}
