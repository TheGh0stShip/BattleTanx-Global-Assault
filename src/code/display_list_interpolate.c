/* SPAN 0x80079AFC */
typedef struct { unsigned int w0, w1; } Gfx;
typedef struct { short ob[3]; unsigned short flag; short tc[2]; unsigned char cn[4]; } Vtx_t;
typedef union { Vtx_t v; long long force_structure_alignment; } Vtx;
extern Gfx *func_8007B190(unsigned int);

inline int func_80079570(Gfx *dl) {
    int max = 0;
    int end;
    int i;

    for (i = 0; dl[i].w0 != 0xDF000000; i++) {
        if (*(unsigned char *)&dl[i] == 1) {
            end = dl[i].w1 + ((dl[i].w0 >> 8) & 0xFF0) - (int)dl;
            if (max < end) {
                max = end;
            }
        }
    }
    return max;
}

inline void func_800795D4(Vtx *a, Vtx *b, Vtx *d, int n, int t) {
    int i;

    for (i = 0; i < n; i++) {
        d[i] = a[i];
        d[i].v.ob[0] = a[i].v.ob[0] + (((b[i].v.ob[0] - a[i].v.ob[0]) * t) >> 8);
        d[i].v.ob[1] = a[i].v.ob[1] + (((b[i].v.ob[1] - a[i].v.ob[1]) * t) >> 8);
        d[i].v.ob[2] = a[i].v.ob[2] + (((b[i].v.ob[2] - a[i].v.ob[2]) * t) >> 8);
        d[i].v.cn[0] = a[i].v.cn[0] + (((b[i].v.cn[0] - a[i].v.cn[0]) * t) >> 8);
        d[i].v.cn[1] = a[i].v.cn[1] + (((b[i].v.cn[1] - a[i].v.cn[1]) * t) >> 8);
        d[i].v.cn[2] = a[i].v.cn[2] + (((b[i].v.cn[2] - a[i].v.cn[2]) * t) >> 8);
    }
}

Gfx *func_800796F0(Gfx *a, Gfx *b, int t) {
    Gfx *d;
    int i = 0;

    d = func_8007B190((unsigned int)func_80079570(a) >> 3);
    if (d == 0) {
        return 0;
    }
    for (; a[i].w0 != 0xDF000000; i++) {
        d[i] = a[i];
        if (*(unsigned char *)&a[i] == 1) {
            d[i].w1 = a[i].w1 - (unsigned int)a + (unsigned int)d;
            func_800795D4((Vtx *)a[i].w1, (Vtx *)b[i].w1, (Vtx *)d[i].w1, (a[i].w0 >> 12) & 0xFF, t);
        }
    }
    d[i] = a[i];
    return d;
}
typedef struct { int a; int c; Gfx *dl; } Part;
typedef struct { Part *parts; unsigned char count; } Model;
extern unsigned char D_802194A5;
extern Gfx *func_8007B0E4(int, int, Gfx *);
extern void func_8007B1F0(int, Gfx *, int, int, int, int, int, int, int, unsigned char);

inline int func_8007995C(int t) {
    return t > 128;
}

void func_80079968(Model **ma, Model **mb, int t, int a3, int arg4, int arg5, int arg6, unsigned char mask, int arg8, int n) {
    int players = D_802194A5;
    Model *A;
    Model *B;
    Part *pa;
    Part *pb;
    Part *src;
    Gfx *g;
    int k;
    int i;

    if (ma != 0) {
        if (mb != 0) {
            if (mask != 0) {
                A = *ma;
                B = *mb;
                for (k = 0; k < A->count; k++) {
                    pa = &A->parts[k];
                    pb = &B->parts[k];
                    src = pa;
                    if (func_8007995C(t)) {
                        src = pb;
                    }
                    g = func_800796F0(pa->dl, pb->dl, t);
                    if (g == 0) {
                        g = pa->dl;
                    }
                    if (n != 0) {
                        g = func_8007B0E4(arg8, n, g);
                    }
                    for (i = 0; i < players; i++) {
                        if ((mask >> i) & 1) {
                            func_8007B1F0(src->a, g, src->c, a3, arg6, arg4, 0, i, arg5, n);
                        }
                    }
                }
            }
        }
    }
}
