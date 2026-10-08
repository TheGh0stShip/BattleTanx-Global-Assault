extern short D_8011F1F8[];
extern void func_800CABC0(unsigned short, char **);
void func_800CB0C0(unsigned short a) {
    char *p;
    func_800CABC0(a, &p);
    *p = 1;
    D_8011F1F8[a] = 0;
}
