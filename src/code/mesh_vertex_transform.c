/* ---- 0x800F2000/b/f3d04.c ---- */
typedef struct { short x, y, z, w; short s, t; unsigned char c[4]; } Vtx3D;
typedef struct { int count; int pad; char *pts; } Mesh3D;
extern void func_8009F288(void *, void *, float *);
void func_800F3D04(Mesh3D *m, void *mtx, Vtx3D *v, unsigned char *col, short *bb) {
    float out[3];
    int i;
    for (i = 0; i < m->count; i++) {
        func_8009F288(mtx, m->pts + i * 12, out);
        v[i].x = out[0];
        v[i].y = out[2];
        v[i].z = out[1];
        v[i].w = 0;
        v[i].c[0] = col[0];
        v[i].c[1] = col[1];
        v[i].c[2] = col[2];
        v[i].c[3] = col[3];
        if (bb[2] < v[i].x) bb[2] = v[i].x;
        if (v[i].x < bb[0]) bb[0] = v[i].x;
        if (bb[3] < v[i].z) bb[3] = v[i].z;
        if (v[i].z < bb[1]) bb[1] = v[i].z;
    }
}

/* ---- 0x800F2000/r2b/f3e58.c ---- */
typedef struct { short x, y, z, f; short s, t; unsigned char c[4]; } Vx;
typedef struct { short v[8]; } Src;
void func_800F3E58(int *n, Vx *out, Src *in, float *off, unsigned char *col, short *bb) {
    int i;
    short t;
    for (i = 0; i < *n; i++) {
        out[i].x = in[i].v[0] + off[0];
        out[i].y = in[i].v[1] + off[2];
        out[i].z = in[i].v[2] + off[1];
        out[i].f = 0;
        out[i].c[0] = col[0];
        out[i].c[1] = col[1];
        out[i].c[2] = col[2];
        out[i].c[3] = col[3];
        if (bb[2] < out[i].x) bb[2] = out[i].x;
        if (out[i].x < bb[0]) bb[0] = out[i].x;
        if (bb[3] < out[i].z) bb[3] = out[i].z;
        if (out[i].z < bb[1]) bb[1] = out[i].z;
    }
}

