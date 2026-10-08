/* ---- 0x800E9000/c/eb934.c ---- */
typedef struct { char pad[28]; int t; } Obj;
extern int D_8021945C;
typedef struct { short x, y; int a, b, c; } V;
void func_800EB934(Obj *a0, int *a1) {
    if (D_8021945C - a0->t >= 6) {
        *a1 = 1;
    }
}
void func_800EB95C(V *src, V *dst, int n, signed char *d) {
    int i;
    for (i = 0; i < n; i++) {
        dst[i] = src[i];
        switch (dst[i].y) {
        case 144: dst[i].x += d[0] * 3; dst[i].y += d[1] * 3; break;
        case 288: dst[i].x += d[2] * 3; dst[i].y += d[3] * 3; break;
        case 432: dst[i].x += d[4] * 3; dst[i].y += d[5] * 3; break;
        case 576: dst[i].x += d[6] * 3; dst[i].y += d[7] * 3; break;
        }
    }
}

