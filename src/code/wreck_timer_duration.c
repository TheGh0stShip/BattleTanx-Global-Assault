/* ---- 0x800E0000/f3404.c ---- */
typedef struct { char pad[0x20]; unsigned char b20; } Wreck5;
extern int D_8023A060;

int func_800E3404(Wreck5 *o) {
    int t = (float)(D_8023A060 * D_8023A060) * 0.92f;
    if (o->b20 == 1) {
        t /= 3;
    }
    return t;
}

