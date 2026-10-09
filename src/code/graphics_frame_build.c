/* SPAN 0x8007A704 */
/* CFLAGS -O0 -G0 -mips3 -mgp32 -mfp32 */
/* LDSYM D_803DA800=0x803DA800 */
/* LDSYM D_01000080=0x01000080 */
typedef struct { unsigned int w0, w1; } Gwords;
typedef union { Gwords words; long long force_structure_alignment; } Gfx;
/* expansion shape of libultra gbi.h (gDma1p / gSPNoOp family) */
#define gSPDisplayList(pkt, dl) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = 0xDE000000; _g->words.w1 = (unsigned int)(dl); }
#define gSPEndDisplayList(pkt) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = 0xDF000000; _g->words.w1 = 0; }
typedef struct { Gfx *a; unsigned char *b; unsigned char *c; void *d; } Bufs;
typedef struct {
    char slot[0x90];
    Bufs bufs[2];
    short id[4];
    int fbsize;
    void *fb;
    unsigned short cnt;
    int unkC4;
    struct { Gfx *p0; Gfx *p1; } dl;
} GfxState;
extern GfxState *D_801144F8;
extern void func_8007A818(void);
extern int func_8007A720(int);
extern Gfx *func_8007A250(Bufs *, Gfx *);
extern Gfx *func_8007A8F0(Bufs *, Gfx *);
#define OS_K0_TO_PHYSICAL(x) (unsigned int)(((char *)(x) - 0x80000000))
#define GPACK_RGBA5551(r, g, b, a) ((((r) << 8) & 0xf800) | (((g) << 3) & 0x7c0) | (((b) >> 2) & 0x3e) | ((a) & 0x1))
#define gWords(pkt, a, b) { Gfx *_g = (Gfx *)(pkt); _g->words.w0 = (a); _g->words.w1 = (unsigned int)(b); }
#define gSPSegment(pkt, seg, base) gWords(pkt, 0xDB060000 | ((seg) * 4), base)
#define gDPSetScissor(pkt, ulx, uly, lrx, lry) gWords(pkt, 0xED000000 | ((ulx) << 14) | ((uly) << 2), ((lrx) << 14) | ((lry) << 2))
#define gDPSetDepthImage(pkt, i) gWords(pkt, 0xFE000000, i)
#define gDPSetColorImage(pkt, w, i) gWords(pkt, 0xFF100000 | ((w) - 1), i)
#define gDPPipeSync(pkt) gWords(pkt, 0xE7000000, 0)
#define gDPSetFillColor(pkt, c) gWords(pkt, 0xF7000000, c)
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) gWords(pkt, 0xF6000000 | ((lrx) << 14) | ((lry) << 2), ((ulx) << 14) | ((uly) << 2))
extern unsigned char D_801144F4, D_801144F5, D_801144F6, D_801144F7;
extern unsigned char D_801294B8;
extern char *D_801144F0;
extern unsigned short D_803DA800[];
extern Gfx D_01000080[];
extern int func_800B0444(void);

int func_8007A0A0(void) {
    Bufs *b;
    struct { Gfx *p0; Gfx *p1; } *dl;
    Gfx *g;
    int ret;

    func_8007A818();
    ret = func_8007A720(D_801144F8->id[0]);
    b = &D_801144F8->bufs[D_801144F8->cnt];
    dl = &D_801144F8->dl;
    g = b->a;
    g = func_8007A250(b, g);
    gSPEndDisplayList(dl->p0++);
    gSPEndDisplayList(dl->p1++);
    if ((unsigned int)dl->p0 > (unsigned int)(b->b + 0xAEE0)) {
    }
    if ((unsigned int)dl->p1 > (unsigned int)(b->c + 0x400)) {
    }
    gSPDisplayList(g++, b->b);
    gSPDisplayList(g++, b->c);
    g = func_8007A8F0(b, g);
    return ret;
}

Gfx *func_8007A250(Bufs *b, Gfx *g) {
    gSPSegment(g++, 0, 0);
    gSPSegment(g++, 1, OS_K0_TO_PHYSICAL(D_801144F0));
    gSPSegment(g++, 2, OS_K0_TO_PHYSICAL(b));
    gDPSetScissor(g++, 0, 0, 320, 240);
    gWords(g++, 0xE200001C, 0);
    gDPSetDepthImage(g++, OS_K0_TO_PHYSICAL(D_803DA800));
    gDPPipeSync(g++);
    gWords(g++, 0xE3000A01, 0x300000);
    gDPSetColorImage(g++, 320, OS_K0_TO_PHYSICAL(D_803DA800));
    gDPSetFillColor(g++, 0xFFFCFFFC);
    if (D_801294B8) {
        gDPFillRectangle(g++, 0, 0, 319, 239);
    }
    gDPPipeSync(g++);
    gDPSetColorImage(g++, 320, OS_K0_TO_PHYSICAL(func_8007A720(D_801144F8->id[0])));
    gDPSetFillColor(g++, (GPACK_RGBA5551(D_801144F4, D_801144F5, D_801144F6, 1) << 16) | GPACK_RGBA5551(D_801144F4, D_801144F5, D_801144F6, 1));
    gDPFillRectangle(g++, 0, 0, 319, 239);
    gDPPipeSync(g++);
    if (func_800B0444()) {
        gWords(g++, 0xD9FFFFFF, 0x10000);
        gWords(g++, 0xF8000000, (D_801144F4 << 24) | (D_801144F5 << 16) | (D_801144F6 << 8) | D_801144F7);
        gWords(g++, 0xDB080000, 0x64009D00);
    }
    gWords(g++, 0xE3000A01, 0);
    gWords(g++, 0xDE000000, D_01000080);
    return g;
}
