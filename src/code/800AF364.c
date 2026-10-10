/* SPAN 0x800AF43C */
extern void *D_803A53A0[];

unsigned int func_8009D914(void);

void func_800AF364(void) {
    void *tmp;
    unsigned int r;
    int i;

    for (i = 0; i < 263; i++) {
        if (i != 21 && D_803A53A0[i] != 0) {
            do {
                r = func_8009D914() % 263;
            } while (r == 21 || D_803A53A0[r] == 0);
            tmp = D_803A53A0[i];
            D_803A53A0[i] = D_803A53A0[r];
            D_803A53A0[r] = tmp;
        }
    }
}
