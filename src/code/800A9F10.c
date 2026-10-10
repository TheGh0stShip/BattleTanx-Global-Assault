/* SPAN 0x800AA058 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u32;

typedef struct { u32 w0, w1; } Gfx;
typedef struct {
    s8 v[3];
} Byte3;

typedef struct {
    Byte3 col;
    char pad1;
    Byte3 colc;
    char pad2;
    Byte3 dir;
    char pad3;
    char pad4[4];
} Light;

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define gSPLight(pkt, l, n) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = _SHIFTL(0xDC, 24, 8) | _SHIFTL((sizeof(Light) - 1) >> 3, 19, 5) | \
               _SHIFTL(((n) * 24 + 24) / 8, 8, 8) | _SHIFTL(0x0A, 0, 8); \
      _g->w1 = (u32)(l); }
#define gSPNumLights(pkt, n) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = _SHIFTL(0xDB, 24, 8) | _SHIFTL(0x02, 16, 8) | _SHIFTL(0, 0, 16); \
      _g->w1 = (u32)((n) * 24); }
#define gSPEndDisplayList(pkt) \
    { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(0xDF, 24, 8); _g->w1 = 0; }

typedef struct {
    Byte3 col;
    Byte3 dir;
} LightSrc;

typedef struct {
    Byte3 col;
    char pad1;
    Byte3 colc;
    char pad2;
} Ambient;

typedef struct {
    Byte3 col;          /* 0x000 */
    char p3[0x104 - 3];
    Gfx *dl;            /* 0x104 */
    Ambient *amb;       /* 0x108 */
} AreaLight;

typedef struct {
    u8 n;
    LightSrc lights[2];
    AreaLight areas[1];
} LightSet;

extern LightSet D_80236B20;

void *func_8007B190(int n);
void func_8007ACF8(Gfx *dl);

void func_800A9D78(void);
typedef struct {
    char p0[0xC4];
    u8 nAreas;          /* 0xC4 */
} World;

typedef struct {
    World *world;       /* 0x80219498 */
} GameState;

extern GameState D_80219498;



void func_800A9F10(void) {
    AreaLight *a;
    Ambient *amb;
    Gfx *dl;
    int i;

    for (i = 0; i < D_80219498.world->nAreas; i++) {
        a = &D_80236B20.areas[i];
        amb = func_8007B190(1);
        if (amb == 0) {
            return;
        }
        dl = func_8007B190(3);
        if (dl == 0) {
            return;
        }
        a->amb = amb;
        amb->col = a->col;
        amb->colc = a->col;
        gSPNumLights(&dl[0], D_80236B20.n);
        gSPLight(&dl[1], amb, D_80236B20.n + 1);
        gSPEndDisplayList(&dl[2]);
        a->dl = dl;
    }
    func_800A9D78();
}
