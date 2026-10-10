/* SPAN 0x800BD880 */
/* RODATA_VRAM 0x80073190 */
/* LDSYM D_80116848=0x80116848 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct { u32 w0, w1; } Gfx;
typedef struct { long m[4][4]; } Mtx;
typedef struct { float m[4][4]; } Matrix4f;
typedef struct { u8 col[3]; char p1; u8 colc[3]; char p2; } Ambient;
typedef struct { u8 col[3]; char p1; u8 colc[3]; char p2; s8 dir[3]; char p3; char pad[4]; } Light;
typedef struct { Ambient a; Light l[1]; } Lights1;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define gSPMatrix(pkt, m, p) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = _SHIFTL(0xDA, 24, 8) | _SHIFTL((sizeof(Mtx) - 1) / 8, 19, 5) | _SHIFTL((p) ^ 0x01, 0, 8); \
      _g->w1 = (u32)(m); }
#define gSPDisplayList(pkt, dl) \
    { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xDE, 24, 8); _g->w1 = (u32)(dl); }
#define gMoveWd(pkt, index, offset, data) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = _SHIFTL(0xDB, 24, 8) | _SHIFTL((index), 16, 8) | _SHIFTL((offset), 0, 16); \
      _g->w1 = (u32)(data); }
#define gSPPerspNormalize(pkt, s) gMoveWd(pkt, 0x0E, 0, (s))
#define gSPNumLights(pkt, n) gMoveWd(pkt, 0x02, 0, (n) * 24)
#define gSPLight(pkt, l, n) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = _SHIFTL(0xDC, 24, 8) | _SHIFTL((sizeof(Light) - 1) >> 3, 19, 5) | \
               _SHIFTL(((n) * 24 + 24) / 8, 8, 8) | _SHIFTL(0x0A, 0, 8); \
      _g->w1 = (u32)(l); }
#define gSPSetLights1(pkt, name) \
    { gSPNumLights(pkt, 1); gSPLight(pkt, &name.l[0], 1); gSPLight(pkt, &name.a, 2); }
#define OS_K0_TO_PHYSICAL(x) ((u32)(x) - 0x80000000)
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = (_SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
      _g->w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); }
#define gDPScisFillRectangle(pkt, ulx, uly, lrx, lry) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = (_SHIFTL(0xF6, 24, 8) | _SHIFTL(MAX((lrx), 0), 14, 10) | _SHIFTL(MAX((lry), 0), 2, 10)); \
      _g->w1 = (_SHIFTL(MAX((ulx), 0), 14, 10) | _SHIFTL(MAX((uly), 0), 2, 10)); }

typedef struct {
    s16 flip;
    float sx;
    float sy;
} HudScale;

typedef struct {
    u16 w, h;
    u8 r, g, b, a;
} HudRect;

typedef struct {
    void *value;
    u16 flags;
} HudNumber;

typedef struct {
    void *text;
    u16 w, h;
} HudText;

typedef struct {
    u8 *tex;
    s16 x0, y0, x1, y1;
} HudImage;

typedef struct {
    char p0[8];
    u16 frame;
    char pA[2];
    u8 **frames;
} HudAnim;

extern u16 D_803A5940;
extern u16 D_803A5942;
extern Gfx *D_803A5944;
extern float D_803A594C;
extern float D_803A5950;
extern u16 D_80114700;

void func_8007BDFC(Gfx **gdl, int mode);
void func_80096F48(Gfx **gdl, void *s, u16 flip, s16 x, s16 y, float sx, float sy);
void func_80096BDC(Gfx **gdl, int v, u16 flip, s16 x, s16 y, float sx, float sy, int fmt);
void func_80096E14(Gfx **gdl, int v, u16 flip, s16 x, s16 y, float sx, float sy, int fmt);
void func_8009700C(Gfx **gdl, void *s, u16 flip, s16 x, s16 y, float sx, float sy, u16 w, u16 h, int c);
void func_8007C9B8(Gfx **gdl, u8 *tex, s16 x, s16 y, float sx, float sy);
void func_8007CE64(Gfx **gdl, u8 *tex, s16 x, s16 y, float sx, float sy, s16 x0, s16 y0, s16 x1, s16 y1);
void func_8007D39C(Gfx **gdl, HudAnim *a, s16 x, s16 y, float sx, float sy);

#define hudMode(m) \
    if (D_803A5940 != (m)) { \
        func_8007BDFC(&D_803A5944, (m)); \
        D_803A5940 = (m); \
    }

static inline u16 texMode(u8 *tex) {
    if (tex == 0) {
        return 0;
    }
    if (*tex == 3 || *tex == 4) {
        return 2;
    }
    return 1;
}

inline void func_800BBDC0(HudScale *p) {
    s16 v = p->flip;

    if (v < 0) {
        D_803A5942 = ~v;
        D_80114700 = 1;
    } else {
        D_803A5942 = v;
        D_80114700 = 0;
    }
    D_803A594C = p->sx;
    D_803A5950 = p->sy;
}

static inline void fillRect(u8 r, u8 g, u8 b, u8 a, s16 x, s16 y, u16 w, u16 h) {
    Gfx *gp = D_803A5944;

    gDPSetPrimColor(gp++, 0, 0, r, g, b, a);
    gDPScisFillRectangle(gp++, x, y, x + w, y + h);
    D_803A5944 = gp;
}

inline void func_800BBE24(HudRect *p, s16 x, s16 y) {
    if (p->a != 0) {
        hudMode(3);
        fillRect(p->r, p->g, p->b, p->a, x, y, p->w, p->h);
    }
}

inline void func_800BBF74(void *s, s16 x, s16 y) {
    hudMode(2);
    func_80096F48(&D_803A5944, s, D_803A5942, x, y, D_803A594C, D_803A5950);
}

void func_800BC020(HudNumber *p, s16 x, s16 y) {
    int v;

    if (p->value != 0) {
        hudMode(2);
        switch (p->flags & 0xF0) {
        case 0x30:
            v = *(u16 *)p->value;
            break;
        case 0x20:
            v = *(s16 *)p->value;
            break;
        case 0x50:
            v = *(u8 *)p->value;
            break;
        case 0x40:
            v = *(s8 *)p->value;
            break;
        case 0x10:
        case 0x00:
        default:
            v = *(int *)p->value;
            break;
        }
        if (p->flags & 0x8000) {
            func_80096E14(&D_803A5944, v, D_803A5942, x, y, D_803A594C, D_803A5950, p->flags & 3);
        } else {
            func_80096BDC(&D_803A5944, v, D_803A5942, x, y, D_803A594C, D_803A5950, p->flags & 3);
        }
    }
}

inline void func_800BC1D4(HudText *p, s16 x, s16 y) {
    if (p->text != 0) {
        hudMode(2);
        func_8009700C(&D_803A5944, p->text, D_803A5942, x, y, D_803A594C, D_803A5950, p->w, p->h, -1);
    }
}

inline void func_800BC2A4(void *t, s16 x, s16 y) {
    u8 *tex = t;
    u16 m = texMode(tex);

    hudMode(m);
    func_8007C9B8(&D_803A5944, tex, x, y, D_803A594C, D_803A5950);
}

inline void func_800BC370(u8 *tex, s16 x, s16 y) {
    hudMode(4);
    func_8007C9B8(&D_803A5944, tex, x, y, D_803A594C, D_803A5950);
}

inline void func_800BC410(HudImage *p, s16 x, s16 y) {
    u16 m = texMode(p->tex);

    hudMode(m);
    func_8007CE64(&D_803A5944, p->tex, x, y, D_803A594C, D_803A5950, p->x0, p->y0, p->x1, p->y1);
}

inline void func_800BC500(HudImage *p, s16 x, s16 y) {
    hudMode(4);
    func_8007CE64(&D_803A5944, p->tex, x, y, D_803A594C, D_803A5950, p->x0, p->y0, p->x1, p->y1);
}

inline void func_800BC5C0(HudAnim *p, s16 x, s16 y) {
    u16 m = texMode(p->frames[p->frame]);

    hudMode(m);
    func_8007D39C(&D_803A5944, p, x, y, D_803A594C, D_803A5950);
}

inline void func_800BC6A0(u8 *c) {
    gDPSetPrimColor(D_803A5944++, 0, 0, c[0], c[1], c[2], c[3]);
}

typedef struct {
    float eye[3];
    float at[3];
} HudView;

typedef struct {
    void *a;
    void *b;
    Gfx *c;
} Part;

typedef struct {
    Part *parts;
    u8 n;
} Lod;

typedef struct {
    float x, y, z;
    u16 ang;
    u16 obj;
} HudModel;

typedef struct {
    int model;
    char p4[0xD0 - 4];
} ObjDef;

extern Mtx *D_803A5930;
extern Lights1 D_80116848;
extern Lod **D_803A53A0[];
extern ObjDef D_80122E94[];
extern Gfx D_1000348[];

void guPerspectiveF(float mf[4][4], u16 *perspNorm, float fovy, float aspect, float near, float far, float scale);
void guLookAtF(float mf[4][4], float xEye, float yEye, float zEye, float xAt, float yAt, float zAt,
               float xUp, float yUp, float zUp);
void guMtxF2L(float mf[4][4], Mtx *m);
void func_8009F4B4(Matrix4f *a, Matrix4f *b, Matrix4f *out);
void func_8009EEE0(Matrix4f *m);
void func_8009EFD4(Matrix4f *m, float x, float y, float z, u16 ang);

void func_800BC6EC(HudView *v) {
    Matrix4f proj;
    Matrix4f view;
    Matrix4f m;
    u16 persp;
    Mtx *mtx;

    guPerspectiveF(proj.m, &persp, 37.0f, 4.0f / 3.0f, 16.0f, 4096.0f, 1.0f);
    guLookAtF(view.m, v->eye[0], v->eye[1], v->eye[2], v->at[0], v->at[1], v->at[2], 0.0f, 1.0f, 0.0f);
    func_8009F4B4(&view, &proj, &m);
    mtx = D_803A5930++;
    guMtxF2L(m.m, mtx);
    gSPPerspNormalize(D_803A5944++, persp);
    gSPMatrix(D_803A5944++, OS_K0_TO_PHYSICAL(mtx), 0x06);
    gSPSetLights1(D_803A5944++, D_80116848);
}

inline void func_800BC874(HudModel *p) {
    Matrix4f f;
    Mtx *mtx;
    Lod **model;
    Lod *lod;
    u16 i;

    D_803A5940 = 0xFFFF;
    func_8009EEE0(&f);
    func_8009EFD4(&f, p->x, p->y, p->z, p->ang);
    mtx = D_803A5930++;
    guMtxF2L(f.m, mtx);
    gSPMatrix(D_803A5944++, OS_K0_TO_PHYSICAL(mtx), 0x02);
    model = D_803A53A0[D_80122E94[p->obj].model];
    if (model == 0 || model == (Lod **)-1) {
        return;
    }
    lod = *model;
    for (i = 0; i < lod->n; i++) {
        Part *pt = &lod->parts[i];

        gSPDisplayList(D_803A5944++, D_1000348);
        if (pt->c != 0) {
            gSPDisplayList(D_803A5944++, pt->c);
        }
    }
}

typedef struct {
    u8 kind;            /* 0x00 */
    u8 color;           /* 0x01 */
    s16 x;              /* 0x02 */
    s16 y;              /* 0x04 */
    char p6[2];
    void *data;         /* 0x08 */
    char pC[4];
} HudElem;

typedef struct {
    u16 *hidden;        /* 0x00 */
    HudElem *elems;     /* 0x04 */
    u8 *colors;         /* 0x08 */
    char pC[0x16 - 0xC];
    s16 x;              /* 0x16 */
    s16 y;              /* 0x18 */
} HudTree;

typedef struct {
    u16 h;
    char p2[14];
} HudFont;

extern Gfx D_1000138[];
extern HudFont D_801B4450[];

int func_800973E0(void *s, u16 flip, float sx);

void func_800BC9F4(HudTree *t) {
    HudElem *e;
    u16 cur;
    u8 r;
    u8 g;
    u8 bl;
    u8 a;
    s16 x;
    s16 y;
    u16 ex;
    u16 ey;

    if (t->hidden != 0 && *t->hidden != 0) {
        return;
    }
    D_803A5942 = 0;
    D_80114700 = 0;
    gSPDisplayList(D_803A5944++, D_1000138);
    e = t->elems;
    D_803A5940 = 0xFFFF;
    D_803A594C = D_803A5950 = 1.0f;
    cur = 0xFFFF;
    while (e->kind != 0) {
        if (e->color != 0 && e->color != cur) {
            switch (e->color) {
            case 0x10:
                r = t->colors[0];
                g = t->colors[1];
                bl = t->colors[2];
                a = t->colors[3];
                break;
            case 0x90:
                r = t->colors[4];
                g = t->colors[5];
                bl = t->colors[6];
                a = t->colors[7];
                break;
            case 0x01:
                r = t->colors[8];
                g = t->colors[9];
                bl = t->colors[10];
                a = t->colors[11];
                break;
            case 0x02:
                r = t->colors[12];
                g = t->colors[13];
                bl = t->colors[14];
                a = t->colors[15];
                break;
            case 0x03:
                r = t->colors[16];
                g = t->colors[17];
                bl = t->colors[18];
                a = t->colors[19];
                break;
            case 4:
            default:
                r = t->colors[20];
                g = t->colors[21];
                bl = t->colors[22];
                a = t->colors[23];
                break;
            }
            gDPSetPrimColor(D_803A5944++, 0, 0, r, g, bl, a);
            cur = e->color;
        }
        switch (e->kind) {
        case 1:
            break;
        case 2:
            func_800BBDC0(e->data);
            break;
        case 3:
            func_800BBE24(e->data, t->x + e->x, t->y + e->y);
            cur = 0xFFFF;
            break;
        case 4:
            if (e->data != 0) {
                func_800BBF74(e->data, t->x + e->x, t->y + e->y);
            }
            break;
        case 5:
            if (e->data != 0) {
                ex = e->x;
                func_800BBF74(e->data, t->x + (ex - func_800973E0(e->data, D_803A5942, D_803A594C)), t->y + e->y);
            }
            break;
        case 6:
            if (e->data != 0) {
                ex = e->x;
                func_800BBF74(e->data, t->x + (ex - ((s16)func_800973E0(e->data, D_803A5942, D_803A594C) >> 1)),
                              t->y + e->y);
            }
            break;
        case 7:
            if (e->data != 0) {
                ex = e->x;
                ey = e->y;
                x = ex - ((s16)func_800973E0(e->data, D_803A5942, D_803A594C) >> 1);
                y = ey - ((s16)(D_801B4450[D_803A5942].h * D_803A5950) >> 1);
                func_800BBF74(e->data, t->x + x, t->y + y);
            }
            break;
        case 8:
            if (e->data != 0) {
                func_800BC020(e->data, t->x + e->x, t->y + e->y);
            }
            break;
        case 9:
            if (e->data != 0) {
                func_800BC1D4(e->data, t->x + e->x, t->y + e->y);
            }
            break;
        case 10: {
            u8 *tx;

            tx = e->data;
            if (tx != 0) {
                func_800BC2A4(tx, t->x + e->x, t->y + e->y);
            }
            break;
        }
        case 15:
            if (e->data != 0) {
                func_800BC370(e->data, t->x + e->x, t->y + e->y);
            }
            break;
        case 13: {
            u8 *tx;

            if (e->data != 0) {
                tx = *(u8 **)e->data;
                if (tx != 0) {
                    func_800BC2A4(tx, t->x + e->x, t->y + e->y);
                }
            }
            break;
        }
        case 11: {
            u8 *tx;

            tx = e->data;
            if (tx != 0) {
                ex = e->x;
                func_800BC2A4(tx, t->x + (ex - ((s16)(((u16 *)tx)[1] * D_803A594C) >> 1)), t->y + e->y);
            }
            break;
        }
        case 12: {
            u8 *tx;

            tx = e->data;
            if (tx != 0) {
                ex = e->x;
                ey = e->y;
                func_800BC2A4(tx, t->x + (ex - ((s16)(((u16 *)tx)[1] * D_803A594C) >> 1)), t->y + (ey - ((s16)(((u16 *)tx)[2] * D_803A5950) >> 1)));
            }
            break;
        }
        case 14: {
            u8 *tx;

            if (e->data != 0) {
                tx = *(u8 **)e->data;
                if (tx != 0) {
                    ex = e->x;
                    func_800BC2A4(tx, t->x + (ex - ((s16)(((u16 *)tx)[1] * D_803A594C) >> 1)), t->y + e->y);
                }
            }
            break;
        }
        case 16:
            if (e->data != 0) {
                func_800BC410(e->data, t->x + e->x, t->y + e->y);
            }
            break;
        case 17:
            if (e->data != 0) {
                func_800BC500(e->data, t->x + e->x, t->y + e->y);
            }
            break;
        case 18:
            if (e->data != 0) {
                func_800BC5C0(e->data, t->x + e->x, t->y + e->y);
            }
            break;
        case 19:
            if (e->data != 0) {
                func_800BC6A0(e->data);
                cur = 0xFFFF;
            }
            break;
        case 20:
            if (e->data != 0) {
                func_800BC6EC(e->data);
            }
            break;
        case 21:
            if (e->data != 0) {
                func_800BC874(e->data);
            }
            break;
        case 22:
            if (e->data != 0) {
                gSPDisplayList(D_803A5944++, e->data);
            }
            break;
        case 23:
            if (e->data != 0) {
                ((void (*)(Gfx **, HudTree *, HudElem *))e->data)(&D_803A5944, t, e);
                D_803A5940 = 0xFFFF;
                cur = 0xFFFF;
            }
            break;
        }
        e++;
    }
    D_80114700 = 0;
}
