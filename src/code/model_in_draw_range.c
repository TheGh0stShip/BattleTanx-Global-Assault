typedef struct { float x, z; char r[32]; } Ent;
typedef struct { char p0; unsigned char n; char p2[14]; Ent e[1]; } Tbl;
extern Tbl D_802194A4;
typedef struct { char pad[48]; float x; float y; float z; } Tank;
typedef struct { char pad[8]; int h; } Inner;
extern void func_800F80E8(int, int *, int *);
extern void func_8007B1F0(int, int, int, int, int, Tank *, int, int, int, int);
static __inline__ int inrange(float *a, float *b, float lim) {
    float dx = a[0] - b[0];
    float dz = a[1] - b[1];
    if (dx * dx + dz * dz > lim) return 0;
    return 1;
}
void func_800F8264(Inner ***h, Tank *t, int id, unsigned char mask) {
    int n = D_802194A4.n;
    int a, b, i;
    float lim;
    if (h == 0) return;
    if (mask == 0) return;
    func_800F80E8(id, &a, &b);
    if (a == 0) return;
    for (i = 0; i < n; i++) {
        if ((mask >> i) & 1) {
            float p[2];
            p[0] = t->x;
            p[1] = t->z;
            if (inrange(&D_802194A4.e[i].x, p, 1e+06f))
                func_8007B1F0(b, (**h)->h, a, 0, 0, t, 0, i, 2, 0);
        }
    }
}
