#define _SHIFTL(v, s, w) ((unsigned int)(((unsigned int)(v) & ((0x01 << (w)) - 1)) << (s)))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) { Gfx *_g = (Gfx *)(pkt); _g->w0 = 0xFA000000 | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8); _g->w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); }
#define gDPScisFillRectangle(pkt, ulx, uly, lrx, lry) { Gfx *_g = (Gfx *)(pkt); _g->w0 = (_SHIFTL(0xF6, 24, 8) | _SHIFTL(MAX((lrx), 0), 14, 10) | _SHIFTL(MAX((lry), 0), 2, 10)); _g->w1 = (_SHIFTL(MAX((ulx), 0), 14, 10) | _SHIFTL(MAX((uly), 0), 2, 10)); }
typedef struct {
    int active;
    void *obj;
    int pad8;
    unsigned short type;
    unsigned char pade;
    unsigned char idx;
} Ev;
typedef struct {
    unsigned char *obj;
    short x;
    short y;
    unsigned short w;
    unsigned short h;
    char pad0C[4];
    unsigned char r, g, b, a;
    char pad14[0xC];
    int unk20;
    char pad24[0x20];
    float unk44;
    float unk48;
    char pad4C[2];
    short unk4E;
    unsigned short flags;
    unsigned char unk52;
    unsigned char ev;
    unsigned char pad54;
    unsigned char next;
    char pad56[2];
} Elem;
typedef struct { unsigned int w0, w1; } Gfx;
extern Ev D_803A66C0[];
extern Elem D_803A6A08[];
extern unsigned short D_80121CE6;
extern unsigned char D_80121CD0;
extern Gfx *D_803A69E4;
extern Gfx *D_803A6FCC;
extern Gfx *D_803A6FD0;
extern char D_01000138[];
extern void func_8007BDFC(Gfx **, int);
extern int func_800973E0(void *, int, float);
extern void func_80096F48(Gfx **, void *, int, int, int, float, float);
extern unsigned short func_8009700C(Gfx **, void *, int, int, int, float, float, int, int, int);
extern void func_8007C9B8(Gfx **, void *, int, int, float, float);
extern void func_8007AD1C(Gfx *);

void func_800D56FC(void)
{
    Elem *e;
    Ev *ev;
    Gfx *g;
    Gfx *g2;
    unsigned char i;
    short x;
    unsigned short mode;

    if (D_80121CE6 == 0) {
        D_803A69E4 = D_803A6FCC;
    } else {
        D_803A69E4 = D_803A6FD0;
    }
    {
    Gfx *g0 = D_803A69E4; D_803A69E4 = g0 + 1;
    g0->w0 = 0xDE000000;
    g0->w1 = (unsigned int)D_01000138;
    }
    for (i = D_80121CD0; i != 255; i = e->next) {
        e = &D_803A6A08[i];
        ev = &D_803A66C0[e->ev];
        if (!ev->active) continue;
        switch (ev->type) {
        case 50:
            if (!(e->flags & 0x4000) || !e->a || !e->obj) break;
            {
            Gfx *gc = D_803A69E4; D_803A69E4 = gc + 1;
            gc->w0 = 0xFA000000;
            gc->w1 = (e->r << 24) | (e->g << 16) | (e->b << 8) | e->a;
            }
            func_8007BDFC(&D_803A69E4, 2);
            switch (e->flags & 3) {
            case 0:
                x = e->x;
                break;
            case 1:
                x = e->x - func_800973E0(e->obj, e->unk52, 1.0f);
                break;
            case 2:
                x = e->x - ((short)func_800973E0(e->obj, e->unk52, 1.0f) >> 1);
                break;
            default:
                x = e->x;
                break;
            }
            func_80096F48(&D_803A69E4, e->obj, e->unk52, x, (short)e->y, 1.0f, 1.0f);
            break;
        case 51:
            if (!(e->flags & 0x4000) || !e->a || !e->obj) break;
            {
            Gfx *gc = D_803A69E4; D_803A69E4 = gc + 1;
            gc->w0 = 0xFA000000;
            gc->w1 = (e->r << 24) | (e->g << 16) | (e->b << 8) | e->a;
            }
            func_8007BDFC(&D_803A69E4, 2);
            if (func_8009700C(&D_803A69E4, e->obj, e->unk52, (short)e->x, (short)e->y, e->unk44, e->unk48,
                              e->w, e->flags & 3, e->unk4E)) {
                e->unk20 = 0;
            }
            break;
        case 52:
            if (!(e->flags & 0x4000) || !e->a || !e->obj) break;
            {
            Gfx *gc = D_803A69E4; D_803A69E4 = gc + 1;
            gc->w0 = 0xFA000000;
            gc->w1 = (e->r << 24) | (e->g << 16) | (e->b << 8) | e->a;
            }
            mode = 0;
            if (e->obj) {
                if ((unsigned)(*e->obj - 3) >= 2) mode = 1; else mode = 2;
            }
            if (mode == 1 && e->a != 255) mode = 4;
            func_8007BDFC(&D_803A69E4, mode);
            func_8007C9B8(&D_803A69E4, e->obj, (short)e->x, (short)e->y, e->unk44, e->unk48);
            break;
        case 53:
            if (!(e->flags & 0x4000) || !e->a) break;
            func_8007BDFC(&D_803A69E4, 3);
            {
                Gfx *p = D_803A69E4;
                short x = e->x;
                unsigned short w = e->w;
                short y = e->y;
                unsigned short h = e->h;
                unsigned char r = e->r, g = e->g, b = e->b, a = e->a;
                gDPSetPrimColor(p++, 0, 0, r, g, b, a);
                gDPScisFillRectangle(p++, x, y, x + w, y + h);
                D_803A69E4 = p;
            }
            break;
        }
    }
    if (D_80121CE6 == 0) {
        g2 = D_803A6FCC;
    } else {
        g2 = D_803A6FD0;
    }
    g = D_803A69E4;
    g->w0 = 0xDF000000;
    g->w1 = 0;
    func_8007AD1C(g2);
    D_80121CE6 = 1 - D_80121CE6;
}
