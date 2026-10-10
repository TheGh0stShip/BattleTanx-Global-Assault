/* SPAN 0x800AC198 */
typedef short s16;

int func_800AAEA0(s16 a, s16 b, s16 c, s16 d, int p, int *x0, float *d0, int *x1, float *d1);
int func_800ABF38(int x0, float d0, int x1, float d1, int layer, int p);

int func_800AC0F4(s16 a, s16 b, s16 c, s16 d, int layer, int p) {
    int x0;
    float d0;
    int x1;
    float d1;

    if (func_800AAEA0(a, b, c, d, p, &x0, &d0, &x1, &d1) != 0) {
        return func_800ABF38(x0, d0, x1, d1, layer, p);
    }
    return 0;
}
