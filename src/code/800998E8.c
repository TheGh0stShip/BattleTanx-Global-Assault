/* SPAN 0x80099F74 */
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct { u32 w0, w1; } Gfx;
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = (_SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
      _g->w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); }
#define gDPFillRectangle(pkt, ulx, uly, lrx, lry) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = (_SHIFTL(0xF6, 24, 8) | _SHIFTL((lrx), 14, 10) | _SHIFTL((lry), 2, 10)); \
      _g->w1 = (_SHIFTL((ulx), 14, 10) | _SHIFTL((uly), 2, 10)); }
#define gSPDisplayList(pkt, dl) \
    { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xDE, 24, 8); _g->w1 = (u32)(dl); }
#define gSPEndDisplayList(pkt) \
    { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xDF, 24, 8); _g->w1 = 0; }

extern Gfx *D_80219490;
extern u8 D_80219460[4][4];
typedef struct {
    void *world;        /* 0x80219498 */
    int track;          /* 0x8021949C */
    unsigned int mode;  /* 0x802194A0 */
    u8 nHuman;          /* 0x802194A4 */
    u8 nPlayers;        /* 0x802194A5 */
} GameState;
extern GameState D_80219498;
extern int D_80114860;
extern u8 D_80219258[][0x100];
extern Gfx D_1000138[];

void func_8007BDFC(Gfx **gdl, int mode);
void func_8007AD1C(void *arg);

static inline void tint(u8 r, u8 gr, u8 b, u8 a, int ulx, int uly, int lrx, int lry) {
    Gfx *g = D_80219490;

    gDPSetPrimColor(g++, 0, 0, r, gr, b, a);
    gDPFillRectangle(g++, ulx, uly, lrx, lry);
    D_80219490 = g;
}

#define TINT(i, ulx, uly, lrx, lry) \
    if (D_80219460[i][3] != 0) { \
        func_8007BDFC(&D_80219490, 3); \
        tint(D_80219460[i][0], D_80219460[i][1], D_80219460[i][2], D_80219460[i][3], ulx, uly, lrx, lry); \
        drawn = 1; \
    }

void func_800998E8(void) {
    int drawn;

    gSPDisplayList(D_80219490, D_1000138);
    D_80219490++;
    drawn = 0;
    switch (D_80219498.nPlayers) {
    case 1:
        TINT(0, 0, 0, 320, 240);
        break;
    case 2:
        TINT(0, 0, 0, 320, 120);
        TINT(1, 0, 120, 320, 240);
        break;
    case 3:
        TINT(0, 0, 0, 320, 120);
        TINT(1, 0, 120, 160, 240);
        TINT(2, 160, 120, 320, 240);
        break;
    case 4:
        TINT(0, 0, 0, 160, 120);
        TINT(1, 160, 0, 320, 120);
        TINT(2, 0, 120, 160, 240);
        TINT(3, 160, 120, 320, 240);
        break;
    }
    if (drawn) {
        gSPEndDisplayList(D_80219490);
        func_8007AD1C(D_80219258[D_80114860]);
    }
}
