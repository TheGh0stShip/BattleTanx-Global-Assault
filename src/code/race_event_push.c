typedef struct { short a, b, c, d; } R;
extern int D_802195CC, D_80117EB4;
extern unsigned short D_8011F218, D_8011F21A, D_8011F1F4;
extern R D_803A6060[];
extern unsigned short func_800C7AA0(unsigned short);
void func_800C8238(short a0, short a1, short a2, unsigned short a3, unsigned short a5, int a6) {
    int s0 = a6 & 0x7FFF;
    int s1 = a6 & 0x8000;
    unsigned short t = a5;
    unsigned short r;
    if (D_802195CC != 3) return;
    if (D_80117EB4 == 1 && D_8011F218 == 0 && D_8011F21A == 0) {
        if (s0 == 0) return;
    }
    if (a3 != 0) return;
    D_803A6060[D_8011F1F4].a = a0;
    D_803A6060[D_8011F1F4].b = a1;
    D_803A6060[D_8011F1F4].c = a2;
    r = func_800C7AA0(t);
    if ((unsigned short)s0 == 1) r |= 0x8000;
    else if ((unsigned short)s0 == 2) r |= 0x4000;
    if (s1) r |= 0x100;
    D_803A6060[D_8011F1F4].d = r;
    D_8011F1F4++;
}
