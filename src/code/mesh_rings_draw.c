typedef struct { short v[4]; } BB;
typedef struct { float m[17]; } MX17;
static const BB D_80077180 = {{ 16000, 16000, -16000, -16000 }};
static const MX17 D_80077188 = {{1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1, 0}};
typedef struct { unsigned int w0, w1; } Gfx;
#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define gSPVertex(pkt, v, n, v0) { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0x01, 24, 8) | _SHIFTL((n), 12, 8) | _SHIFTL((v0) + (n), 1, 7); _g->w1 = (unsigned int)(v); }
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8); _g->w1 = _SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8); }
#define gSP1Triangle(pkt, v0, v1, v2, flag) { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0x05, 24, 8) | (_SHIFTL((v0) * 2, 16, 8) | _SHIFTL((v1) * 2, 8, 8) | _SHIFTL((v2) * 2, 0, 8)); _g->w1 = 0; }
#define gSPEndDisplayList(pkt) { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xDF, 24, 8); _g->w1 = 0; }
typedef struct { int nv; int nseg; int pad8; unsigned char *seg; } Ring;
typedef struct { float m[17]; } Mx;
extern unsigned char D_802194A4[];
extern char D_010003C0[];
extern void *func_8007B190();
extern void func_8009EEE0(void *);
extern void func_8009EFD4(void *, float, float, float, unsigned short);
extern void func_8009F824(void *, float, float, float);
extern void func_800F3D04();
extern void func_800F3E58(Ring *, void *, void *, float *, unsigned char *, short *);
extern unsigned char func_800ACFE0();
extern void func_80102C00(void *, void *, int);
extern int func_8007B1F0();

void func_800F3F78(Ring *o, float *to, float *from, unsigned short ang, float scale, int n,
                   unsigned char *c0, unsigned char *c1, unsigned char alpha)
{
    Gfx *base;
    int ring;
    int flag = 0;
    unsigned char mask;
    int nv = o->nv;
    BB bbs = D_80077180;
#define bb bbs.v
    float rect[8];
    void *first = 0;
    int last = n - 1;
    Mx mtxf = *(Mx *)&D_80077188;
    void *bufs[16];
    unsigned char col[4];
    float off[3];
    float mtx[16];
    int i, nseg;
    Gfx *g;
    int step;

    if (n < 2) return;
    ring = nv * 2;
    nseg = n + (o->nseg * 2 + 1) * last + 1;
    base = func_8007B190(nseg + ring * n);
    g = base;
    if (g == 0) return;
    for (i = 0; i < n; i++) {
        col[0] = c0[0] + (c1[0] - c0[0]) * i / last;
        col[1] = c0[1] + (c1[1] - c0[1]) * i / last;
        col[2] = c0[2] + (c1[2] - c0[2]) * i / last;
        col[3] = c0[3] + (c1[3] - c0[3]) * i / last;
        bufs[i] = g + nseg + i * ring;
        if (i == 0) {
            func_8009EEE0(mtx);
            func_8009EFD4(mtx, from[0], from[2], from[1], ang);
            func_8009F824(mtx, scale, scale, scale);
            func_800F3D04(o, mtx, bufs[0], col, bb);
            first = bufs[0];
        } else {
            off[0] = (to[0] - from[0]) * i / last;
            off[2] = (to[2] - from[2]) * i / last;
            off[1] = (to[1] - from[1]) * i / last;
            func_800F3E58(o, bufs[i], first, off, col, bb);
        }
    }
    rect[0] = bb[0]; rect[1] = bb[1];
    rect[2] = bb[2]; rect[3] = bb[1];
    rect[4] = bb[2]; rect[5] = bb[3];
    rect[6] = bb[0]; rect[7] = bb[3];
    mask = func_800ACFE0(rect, alpha);
    if (mask == 0) return;
    step = 255 / last;
    gSPVertex(g++, bufs[0], nv, 0);
    gSPVertex(g++, bufs[1], nv, nv);
    gDPSetPrimColor(g++, 0, 0, 255, 255, 255, step);
    for (i = 0; i < o->nseg; i++) {
        gSP1Triangle(g++, o->seg[i * 2], o->seg[i * 2 + 1], o->seg[i * 2] + nv, 0);
        gSP1Triangle(g++, o->seg[i * 2] + nv, o->seg[i * 2 + 1] + nv, o->seg[i * 2 + 1], 0);
    }
    for (i = 1; i < last; i++) {
        Gfx *p = g++;
        p->w0 = _SHIFTL(0x01, 24, 8) | _SHIFTL(nv, 12, 8) | (flag ? _SHIFTL(nv + nv, 1, 7) : _SHIFTL(nv, 1, 7));
        p->w1 = (unsigned int)bufs[i + 1];
        gDPSetPrimColor(g++, 0, 0, 255, 255, 255, (i + 1) * 255 / last);
        func_80102C00(base + 3, g, o->nseg * 16);
        g += o->nseg * 2;
        flag ^= 1;
    }
    gSPEndDisplayList(g++);
    for (i = 0; i < D_802194A4[1]; i++) {
        if ((mask >> i) & 1)
            func_8007B1F0(0, base, D_010003C0, 0, 0, &mtxf, 0, i, 1, 0);
    }
}

#undef bb

typedef struct { unsigned int w0, w1; } G;
typedef struct { short x, y, z, w; short s, t; unsigned char c[4]; } Vtx3D;
typedef struct { int nv; int ne; char *pts; unsigned char *edges; } Mesh;
typedef struct { float m[4][4]; int x; } MtxF;
typedef struct { float m[16]; } Mtx64;
extern unsigned char D_802194A4[];
extern char D_010003C0[];
#define SHL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((1 << (w)) - 1)) << (s)))
#define VTX(pkt, v, n, v0) { G *_v = (G *)(pkt); _v->w0 = SHL(1, 24, 8) | SHL((n), 12, 8) | SHL((v0) + (n), 1, 7); _v->w1 = (unsigned int)(v); }
#define PRIM(pkt, a) { G *_p = (G *)(pkt); _p->w0 = SHL(0xFA, 24, 8) | SHL(0, 8, 8) | SHL(0, 0, 8); _p->w1 = SHL(255, 24, 8) | SHL(255, 16, 8) | SHL(255, 8, 8) | SHL((a), 0, 8); }
#define TRIW(a, b, c) (SHL((a) * 2, 16, 8) | SHL((b) * 2, 8, 8) | SHL((c) * 2, 0, 8))
#define TRI1(pkt, a, b, c, f) { G *_t = (G *)(pkt); _t->w0 = SHL(5, 24, 8) | ((f) == 0 ? TRIW(a, b, c) : (f) == 1 ? TRIW(b, c, a) : TRIW(c, a, b)); _t->w1 = 0; }
extern void _bcopy(void *, void *, int);

void func_800F4698(Mesh *m, int n, Mtx64 *mtxs, unsigned char *ca, unsigned char *cb, int off, int mod, unsigned char flag)
{
    G *buf;
    int step;
    int last = n - 1;
    int toggle = 0;
    int nv = m->nv;
    BB bbs = D_80077180;
#define bb bbs.v
    float q[8];
    MtxF mtx = *(MtxF *)&D_80077188;
    Vtx3D *ptrs[16];
    unsigned char col[4];
    int i, ncmd;
    G *g, *vb;
    unsigned char mask;
    unsigned int w;

    if (n < 2) return;
    step = nv * 2;
    ncmd = n + (m->ne * 2 + 1) * last + 1;
    buf = func_8007B190(ncmd + step * n);
    g = buf;
    if (buf == 0) return;
    for (i = 0; i < n; i++) {
        col[0] = ca[0] + (cb[0] - ca[0]) * i / last;
        col[1] = ca[1] + (cb[1] - ca[1]) * i / last;
        col[2] = ca[2] + (cb[2] - ca[2]) * i / last;
        col[3] = ca[3] + (cb[3] - ca[3]) * i / last;
        ptrs[i] = (Vtx3D *)(&buf[ncmd] + i * step);
        func_800F3D04(m, &mtxs[(i + off) % mod], ptrs[i], col, bb);
    }
    q[0] = bb[0]; q[1] = bb[1];
    q[2] = bb[2]; q[3] = bb[1];
    q[4] = bb[2]; q[5] = bb[3];
    q[6] = bb[0]; q[7] = bb[3];
    mask = func_800ACFE0(q, flag);
    if (mask == 0) return;
    VTX(g++, ptrs[0], nv, 0);
    VTX(g++, ptrs[1], nv, nv);
    PRIM(g++, cb[3] * (unsigned char)(255 / last) / 255);
    for (i = 0; i < m->ne; i++) {
        TRI1(g++, m->edges[i * 2], m->edges[i * 2 + 1], m->edges[i * 2] + nv, 0);
        TRI1(g++, m->edges[i * 2] + nv, m->edges[i * 2 + 1] + nv, m->edges[i * 2 + 1], 0);
    }
    for (i = 1; i < last; i++) {
        VTX(g++, ptrs[i + 1], nv, toggle ? nv : 0);
        PRIM(g++, cb[3] * (unsigned char)((i + 1) * 255 / last) / 255);
        _bcopy(buf + 3, g, m->ne * 16);
        g += m->ne * 2;
        toggle ^= 1;
    }
    { G *_e = g++; _e->w0 = SHL(0xDF, 24, 8); _e->w1 = 0; }
    for (i = 0; i < D_802194A4[1]; i++) {
        if ((mask >> i) & 1) {
            func_8007B1F0(0, buf, (int)D_010003C0, 0, 0, &mtx, 0, i, 1, 0);
        }
    }
}
