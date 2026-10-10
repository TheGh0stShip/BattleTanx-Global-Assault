/* RODATA_VRAM 0x80072A00 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct {
    float x;
    float y;
    float z;
} Vec3;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    float t0;
    float t1;
    float t2;
    float s0;
    float s1;
    float s2;
    u8 a0;
    u8 a1;
    u8 a2;
    u8 env[3];
    u8 prim[3];
    int model;
} SprKey;

extern void *D_803A53A0[];

void func_800AD6A8(void *model, Vec3 *pos, float sx, float sy, float sz,
                   u16 angle, int arg6, int arg7, u8 flag, Gfx *commands,
                   int command_count, int mode);

#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define gDPSetColor(pkt, c, d) \
    { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(c, 24, 8); _g->w1 = (u32)(d); }
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = (_SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
      _g->w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); }
#define gDPSetEnvColor(pkt, r, g, b, a) \
    gDPSetColor(pkt, 0xFB, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))

static inline float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

static inline u8 alpha8(float value) {
    return value;
}

void func_800A40CC(SprKey *k, float t, float scale, Vec3 *pos, int unused, u8 flag) {
    float f, s;
    u8 a;
    Gfx cmds[2];
    Gfx *g;

    if (t < k->t0 || k->t2 < t) return;
    if (t < k->t1) {
        f = (t - k->t0) / (k->t1 - k->t0);
        s = lerp(k->s0, k->s1, f) * scale;
        a = alpha8(lerp(k->a0, k->a1, f));
    } else {
        f = (t - k->t1) / (k->t2 - k->t1);
        s = lerp(k->s1, k->s2, f) * scale;
        a = lerp(k->a1, k->a2, f);
    }
    g = cmds;
    gDPSetPrimColor(g++, 0, 0, k->prim[0], k->prim[1], k->prim[2], a);
    gDPSetEnvColor(g++, k->env[0], k->env[1], k->env[2], 0);
    func_800AD6A8(D_803A53A0[k->model], pos, s, s, 0, 0, 0, 0, flag, cmds, 2, 0);
}
