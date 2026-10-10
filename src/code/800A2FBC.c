/* SPAN 0x800A3560 */
/* RODATA_VRAM 0x800729C0 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef unsigned int u32;

typedef struct { float x, y, z; } Vec3;
typedef struct { s8 r, g, b; } Rgb;
typedef struct { char hdr[0xC]; } FxHdr;

typedef struct {
    FxHdr hdr;
    Vec3 pos;       /* 0x0C */
    Vec3 vel;       /* 0x18 */
    Rgb col;        /* 0x24 */
    s8 kind;        /* 0x27 */
    int time;       /* 0x28 */
    u8 count;       /* 0x2C */
    u8 head;        /* 0x2D */
    s16 (*trail)[3]; /* 0x30 */
    float scale;    /* 0x34 */
} FxTrail;

typedef struct {
    FxHdr hdr;
    void *data;     /* 0x0C */
    int time;       /* 0x10 */
    Vec3 pos;       /* 0x14 */
    s8 kind;        /* 0x20 */
    s8 flag;        /* 0x21 */
    char pad22[2];
    float scale;    /* 0x24 */
} FxPoint;

typedef struct {
    FxHdr hdr;
    void *data;     /* 0x0C */
    int time;       /* 0x10 */
    Vec3 pos;       /* 0x14 */
    s16 unk20;
    u16 ang;        /* 0x22 */
    s8 kind;        /* 0x24 */
    s8 side;        /* 0x25 */
    s8 hasTarget;   /* 0x26 */
    char pad27;
    Vec3 target;    /* 0x28 */
    float scale;    /* 0x34 */
} FxAim;

typedef struct {
    FxHdr hdr;
    void *data;     /* 0x0C */
    int time;       /* 0x10 */
    Vec3 pos;       /* 0x14 */
    u16 ang;        /* 0x20 */
    s8 kind;        /* 0x22 */
    s8 side;        /* 0x23 */
    Vec3 vel;       /* 0x24 */
} FxSpin;

typedef struct {
    FxHdr hdr;
    struct FxDef *def; /* 0x0C */
    int time;       /* 0x10 */
    Vec3 pos;       /* 0x14 */
    float scale;    /* 0x20 */
    u16 ang;        /* 0x24 */
    s8 kind;        /* 0x26 */
    s8 flag;        /* 0x27 */
    s8 side;        /* 0x28 */
} FxBurst;

typedef struct FxDef {
    char pad0[0x2A];
    u8 flag;
} FxDef;

typedef struct {
    FxHdr hdr;
    void *data;
    int time;
    s8 kind;        /* 0x14 */
} FxMark;

typedef struct {
    int unk0;
    int min;        /* 0x04 */
    int max;        /* 0x08 */
    Rgb cols[4];    /* 0x0C */
} SparkDef;

typedef struct {
    float time;     /* 0x00 */
    float x, y, z;  /* 0x04 */
    u16 ang;        /* 0x10 */
    int op;         /* 0x14 */
    void *arg;      /* 0x18 */
} FxStep;

typedef struct {
    int unk0;
    FxStep *steps;  /* 0x04 */
} FxScriptDef;

typedef struct {
    u8 min, max;    /* 0x00 */
    u8 back;        /* 0x02 */
    float lo, hi;   /* 0x04 */
} FxLoop;

typedef struct {
    FxHdr hdr;
    FxScriptDef *def; /* 0x0C */
    float t;        /* 0x10 */
    int idx;        /* 0x14 */
    void *arg;      /* 0x18 */
    Vec3 pos;       /* 0x1C */
    u16 ang;        /* 0x28 */
    u8 kind;        /* 0x2A */
    int loop;       /* 0x2C */
    float scale;    /* 0x30 */
} FxScript;

typedef struct {
    int unk0;
    int dur;        /* 0x04 */
    float sx;       /* 0x08 */
    float sz;       /* 0x0C */
    float prob;     /* 0x10 */
    int op;         /* 0x14 */
} FxEmitDef;

typedef struct {
    FxHdr hdr;
    FxEmitDef *def; /* 0x0C */
    int time;       /* 0x10 */
    Vec3 pos;       /* 0x14 */
    u8 kind;        /* 0x20 */
    char pad21[3];
    float scale;    /* 0x24 */
} FxEmit;

extern int D_8021945C;
extern float D_80219488;
extern u8 D_80114E30;

int func_800A19DC(void);
void *func_800A18D0(int type, int size);
void func_800A1AFC(int h);
float func_8009D8A0(float max);
u32 func_8009D914(void);
void func_8009EC0C(float *v);
float func_8009D4B0(u16 a);
float func_8009D510(u16 a);
typedef struct { float m[4][4]; } Mtx;
void func_8009EEE0(Mtx *m);
void func_8009EF30(Mtx *m, float *v);
void func_800F4698(void *mesh, int n, Mtx *mtxs, u8 *ca, u8 *cb, int off, int mod, u8 flag);
extern char D_80114E84[];

typedef struct { u32 w0, w1; } Gfx;
#define _SHIFTL(v, s, w) ((u32)(((u32)(v) & ((1 << (w)) - 1)) << (s)))
#define gDPSetColor(pkt, c, d) \
    { Gfx *_g = (Gfx *)(pkt); _g->w0 = _SHIFTL(c, 24, 8); _g->w1 = (u32)(d); }
#define gDPSetPrimColor(pkt, m, l, r, g, b, a) \
    { Gfx *_g = (Gfx *)(pkt); \
      _g->w0 = (_SHIFTL(0xFA, 24, 8) | _SHIFTL(m, 8, 8) | _SHIFTL(l, 0, 8)); \
      _g->w1 = (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)); }
#define gDPSetEnvColor(pkt, r, g, b, a) \
    gDPSetColor(pkt, 0xFB, (_SHIFTL(r, 24, 8) | _SHIFTL(g, 16, 8) | _SHIFTL(b, 8, 8) | _SHIFTL(a, 0, 8)))
typedef struct {
    float t0, t1, t2;   /* 0x00 */
    float s0, s1, s2;   /* 0x0C */
    u8 a0, a1, a2;      /* 0x18 */
    u8 env[3];          /* 0x1B */
    u8 prim[3];         /* 0x1E */
    int model;          /* 0x24 */
} SprKey;

typedef struct {
    float t0, t1, t2, t3; /* 0x00 */
    u16 spin;           /* 0x10 */
    u8 env[3];          /* 0x12 */
    u8 prim[3];         /* 0x15 */
    int model;          /* 0x18 */
} SprRing;

typedef struct {
    int unk0;
    int dur;            /* 0x04 */
    float scale;        /* 0x08 */
    SprRing *rings;     /* 0x0C */
    SprKey *keys;       /* 0x10 */
} SprDef;

typedef struct {
    FxHdr hdr;
    SprDef *def;        /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    u8 kind;            /* 0x20 */
    u8 rev;             /* 0x21 */
    char pad22[2];
    float scale;        /* 0x24 */
} FxSprite;

typedef struct {
    int unk0;
    u8 r, g, b;         /* 0x04 */
    u8 mode;            /* 0x07 */
    int dur;            /* 0x08 */
} LightDef;

typedef struct {
    FxHdr hdr;
    LightDef *def;      /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    u8 kind;            /* 0x20 */
} FxLight;

typedef struct {
    int unk0;
    u8 prim0[4];        /* 0x04 */
    u8 env0[4];         /* 0x08 */
    u8 prim1[4];        /* 0x0C */
    u8 env1[4];         /* 0x10 */
    float s0, s1;       /* 0x14 */
    float z0, z1;       /* 0x1C */
    int dur;            /* 0x24 */
    u16 spin;           /* 0x28 */
    int model;          /* 0x2C */
} BillDef;

typedef struct {
    FxHdr hdr;
    BillDef *def;       /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    short unk20;
    u16 ang;            /* 0x22 */
    u8 kind;            /* 0x24 */
    s8 dir;             /* 0x25 */
    u8 mode;            /* 0x26 */
    char pad27;
    Vec3 axis;          /* 0x28 */
    float scale;        /* 0x34 */
} FxBill;

typedef struct {
    int unk0;
    int dur;            /* 0x04 */
    float w0, w1;       /* 0x08 */
    float h0, h1;       /* 0x10 */
    u8 prim0[4];        /* 0x18 */
    u8 env0[4];         /* 0x1C */
    u8 prim1[4];        /* 0x20 */
    u8 env1[4];         /* 0x24 */
    int model;          /* 0x28 */
} FlatDef;

typedef struct {
    FxHdr hdr;
    FlatDef *def;       /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    u16 ang;            /* 0x20 */
    u8 kind;            /* 0x22 */
    s8 dir;             /* 0x23 */
    float sx;           /* 0x24 */
    float sy;           /* 0x28 */
} FxFlat;

extern void *D_803A53A0[];
void func_800AD6A8(void *model, Vec3 *pos, float sx, float sy, float sz, u16 ang, int a6, int a7, u8 flag,
                   Gfx *cmds, int n, int a11);
void func_800AF13C(void *model, Vec3 *pos, Vec3 *axis, float s, u16 ang, int a5, int a6, Gfx *cmds, int n,
                   u8 flag);
u8 func_800AD14C(float x, float y, float r, u8 kind);
void func_800A9CCC(u8 kind, Vec3 *pos, u8 r, u8 g, u8 b, u8 mode);

typedef struct FxDefHdr {
    int op;             /* 0x00 */
    int i4;             /* 0x04 */
    float f8;           /* 0x08 */
} FxDefHdr;

typedef struct {
    int op;
    short unk4;
    short s6;           /* 0x06 */
    float f8;           /* 0x08 */
} FxSoundDef;

typedef struct FxChain {
    int op;
    char pad4[0x20];
    int dur;            /* 0x24 */
    u16 spin;           /* 0x28 */
    char pad2A[6];
    struct FxChain *next; /* 0x30 */
} FxChain;

typedef struct {
    FxHdr hdr;
    FxChain *def;       /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    u16 ang0;           /* 0x20 */
    u16 ang;            /* 0x22 */
    u8 kind;            /* 0x24 */
} FxChainObj;

typedef struct FxLaunch {
    int op;
    int dur;            /* 0x04 */
    char pad8[0x24];
    struct FxLaunch *next; /* 0x2C */
} FxLaunch;

typedef struct {
    FxHdr hdr;
    FxLaunch *def;      /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    u8 unk20, unk21;
    u8 kind;            /* 0x22 */
    char pad23[9];
    float accel;        /* 0x2C */
} FxLaunchObj;

typedef struct {
    int op;
    int unk4;
    int dur;            /* 0x08 */
} FxTimedDef;

typedef struct {
    FxHdr hdr;
    FxTimedDef *def;    /* 0x0C */
    int time;           /* 0x10 */
} FxTimed;

typedef struct { u8 r, g, b; } Rgb8;
typedef struct { Rgb8 c[5]; char pad[9]; } FogCfg;

typedef struct {
    int op;
    Rgb8 fog[5];        /* 0x04 */
    char pad13[6];
    Rgb8 light;         /* 0x19 */
    int dur;            /* 0x1C */
} FogDef;

typedef struct {
    FxHdr hdr;
    FogDef *def;        /* 0x0C */
    int time;           /* 0x10 */
    u8 kind;            /* 0x14 */
} FxFog;

typedef struct {
    Rgb8 col;
    char pad[29];
} PlayerLight;

typedef struct {
    char pad0[0x1C];
    PlayerLight pl[4];  /* 0x1C */
    char pad9C[0x26C - 0x9C];
    FogCfg fog;         /* 0x26C */
} World;

typedef struct {
    int op;
    u8 r, g, b;         /* 0x04 */
    char pad7;
    int dur;            /* 0x08 */
} FlashDef;

typedef struct {
    FxHdr hdr;
    FlashDef *def;      /* 0x0C */
    int time;           /* 0x10 */
} FxFlash;

typedef struct SndDef {
    int op;
    char pad4[0x10];
    float v0, v1;       /* 0x14 */
    char pad1C[8];
    int dur;            /* 0x24 */
    char pad28[0xC];
    struct FxDefHdr *next; /* 0x34 */
} SndDef;

typedef struct {
    FxHdr hdr;
    SndDef *def;        /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    float scale;        /* 0x20 */
    char pad24[2];
    u8 kind;            /* 0x26 */
    u8 snd;             /* 0x27 */
} FxSnd;

typedef struct { float m[4][4]; int flag; } Mtx17;

typedef struct {
    int op;
    u8 prim0[4];        /* 0x04 */
    u8 env0[4];         /* 0x08 */
    u8 prim1[4];        /* 0x0C */
    u8 env1[4];         /* 0x10 */
    float s0, s1;       /* 0x14 */
    float h0, h1;       /* 0x1C */
    int dur;            /* 0x24 */
    u16 spin;           /* 0x28 */
    int model;          /* 0x2C */
    int model2;         /* 0x30 */
} MeshDef;

typedef struct {
    FxHdr hdr;
    MeshDef *def;       /* 0x0C */
    int time;           /* 0x10 */
    Vec3 pos;           /* 0x14 */
    float scale;        /* 0x20 */
    u16 ang;            /* 0x24 */
    u8 kind;            /* 0x26 */
    char pad27;
    u8 dir;             /* 0x28 */
} FxMesh;

extern World *D_80219498;
extern u8 D_802194A5;
extern void *D_80114E94[];
extern void *D_80114EA0;

void func_800B04E0(void);
void func_800B044C(FogCfg *cfg);
void func_800A9BF0(u8 kind, u8 r, u8 g, u8 b);
void func_800998A8(int player, u8 r, u8 g, u8 b, u8 a);
void func_800EC4A8(int vol);
void func_800EC1C0(void);
int func_800EC1F8(Vec3 *pos, u8 kind, int a2, int a3, int a4, int a5);
void func_8009EFD4(Mtx17 *m, float x, float z, float y, u16 ang);
void func_8009F824(Mtx17 *m, float sx, float sy, float sz);
void func_800AD9A8(void *model, int a1, Mtx17 *m, int a3, int a4, u8 flag, Gfx *cmds, int n);
void func_800A2C6C(u8 kind, Vec3 *pos, short a2, float a3, int a4, int a5);
void func_800F50B8(Vec3 *pos, u8 kind, int a2, int a3, int a4, float a5);
void func_800A5BD8(Vec3 *pos, u16 ang, u8 kind, float scale, void *def, void *arg);

extern inline u8 func_800A2EF0(void) {
    return D_80114E30 = -D_80114E30;
}

extern inline void func_800A2F0C(Vec3 *pos, u16 ang, u8 kind, void *def, void *arg, float scale) {
    FxScript *o = func_800A18D0(0x34, sizeof(FxScript));

    if (o != 0) {
        o->def = def;
        o->t = 0;
        o->idx = 0;
        o->arg = arg;
        o->pos = *pos;
        o->ang = ang;
        o->kind = kind;
        o->loop = 0;
        o->scale = scale;
    }
}

static inline float frand(float lo, float hi) {
    return lo + func_8009D8A0(hi - lo);
}

void func_800A2FBC(FxScript *o, int *done) {
    FxStep *st;
    FxLoop *l;
    Vec3 v;
    void *arg;
    u16 ang;

    o->t += D_80219488;
    while (o->def->steps[o->idx].time <= o->t) {
        st = &o->def->steps[o->idx];
        switch (st->op) {
        case -1:
            if (o->loop == 1) {
                o->loop = 0;
                o->idx++;
                break;
            }
            l = st->arg;
            if (o->loop == 0) {
                if (l->min == l->max) {
                    o->loop = l->min;
                } else {
                    o->loop = func_8009D914() % (l->max - l->min) + l->min;
                }
            } else if (l->min != 255) {
                o->loop--;
            }
            o->t -= frand(l->lo, l->hi);
            o->idx -= l->back;
            break;
        case 0:
            *done = 1;
            return;
        default:
            v.x = o->pos.x + o->scale * (func_8009D510(o->ang) * st->x - func_8009D4B0(o->ang) * st->y);
            v.y = o->pos.y + o->scale * (func_8009D4B0(o->ang) * st->x + func_8009D510(o->ang) * st->y);
            v.z = o->pos.z + o->scale * st->z;
            ang = o->ang + st->ang;
            arg = st->arg;
            if (arg == (void *)-1) {
                arg = o->arg;
            }
            func_800A5BD8(&v, ang, o->kind, o->scale, (void *)st->op, arg);
            o->idx++;
            break;
        }
    }
}

inline void func_800A3248(Vec3 *pos, s8 kind, Vec3 *vel, Rgb *col, float scale) {
    int h = func_800A19DC();
    FxTrail *o;

    if (h == 0) return;
    o = func_800A18D0(0x2B, sizeof(FxTrail));
    if (o == 0) {
        func_800A1AFC(h);
        return;
    }
    o->col = *col;
    o->time = D_8021945C - 15 + (int)func_8009D8A0(30.0f);
    o->vel = *vel;
    o->head = 0;
    o->count = 0;
    o->trail = (void *)h;
    o->pos = *pos;
    o->kind = kind;
    o->scale = scale;
}

void func_800A3368(FxTrail *o, int *done) {
    if (D_8021945C - o->time > 20) {
        *done = 1;
        func_800A1AFC((int)o->trail);
        return;
    }
    o->trail[o->head][0] = o->pos.x;
    o->trail[o->head][1] = o->pos.z;
    o->trail[o->head][2] = o->pos.y;
    o->head = (o->head + 1) % 7;
    if (++o->count >= 8) {
        o->count = 7;
    }
    o->pos.x += (frand(-3.0f, 3.0f) + o->vel.x) * D_80219488 * o->scale;
    o->pos.z += (frand(-3.0f, 3.0f) + o->vel.z) * D_80219488 * o->scale;
    o->pos.y += (frand(-3.0f, 3.0f) + o->vel.y) * D_80219488 * o->scale;
    o->vel.z -= 1.0f;
}

void func_800A3560(FxTrail *o);

void func_800A3758(Vec3 *pos, u16 ang, u8 kind, SparkDef *def, void *arg, float scale);

extern inline void func_800A396C(Vec3 *pos, u16 ang, u8 kind, void *data, void *arg, float scale) {
    FxPoint *o = func_800A18D0(0x38, sizeof(FxPoint));

    if (o != 0) {
        o->data = data;
        o->time = D_8021945C;
        o->pos = *pos;
        o->kind = kind;
        o->scale = scale;
    }
}

void func_800A39FC(FxEmit *o, int *done);

extern inline void func_800A3B68(Vec3 *pos, u16 a1, u8 kind, void *data, Vec3 *target, float scale) {
    FxAim *o = func_800A18D0(0x35, sizeof(FxAim));

    if (o != 0) {
        o->data = data;
        o->time = D_8021945C;
        o->pos = *pos;
        o->unk20 = a1;
        o->kind = kind;
        o->ang = func_8009D914() % 65535;
        o->scale = scale;
        o->side = D_80114E30 = -D_80114E30;
        if (target != 0) {
            o->hasTarget = 1;
            o->target = *target;
        } else {
            o->hasTarget = 0;
        }
    }
}

extern inline void func_800A3C8C(Vec3 *pos, u16 ang, u8 kind, void *data, Vec3 *vel) {
    FxSpin *o = func_800A18D0(0x37, sizeof(FxSpin));

    if (o != 0) {
        o->data = data;
        o->pos = *pos;
        o->kind = kind;
        o->side = D_80114E30 = -D_80114E30;
        o->ang = func_8009D914() % 65535;
        o->time = D_8021945C;
        o->vel.x = (func_8009D8A0(0.9f) + 0.2f) * vel->x;
        o->vel.y = (func_8009D8A0(0.9f) + 0.2f) * vel->y;
        o->vel.z = (func_8009D8A0(0.9f) + 0.2f) * vel->z;
    }
}

extern inline void func_800A3DC8(Vec3 *pos, u8 kind, float scale, FxDef *def, int flag) {
    FxBurst *o = func_800A18D0(0x17, sizeof(FxBurst));

    if (o != 0) {
        o->def = def;
        o->time = D_8021945C;
        o->pos = *pos;
        o->kind = kind;
        o->ang = func_8009D914() % 65535;
        if (def->flag != 0 & flag != 0) {
            o->flag = 1;
        } else {
            o->flag = 0;
        }
        o->scale = scale;
        o->side = D_80114E30 = -D_80114E30;
    }
}

extern inline void func_800A3ED0(u8 kind, void *data) {
    FxMark *o = func_800A18D0(0x2D, 0x18);

    if (o != 0) {
        o->data = data;
        o->time = D_8021945C;
        o->kind = kind;
    }
}

extern inline void func_800A3F2C(int unused0, void *data) {
    FxMark *o = func_800A18D0(0x2E, 0x14);

    if (o != 0) {
        o->data = data;
        o->time = D_8021945C;
    }
}

extern inline void func_800A3F78(Vec3 *pos, u8 kind, void *data, s8 flag, float scale) {
    FxPoint *o = func_800A18D0(0x33, sizeof(FxPoint));

    if (o != 0) {
        o->data = data;
        o->time = D_8021945C;
        o->pos = *pos;
        o->kind = kind;
        o->flag = flag;
        o->scale = scale;
    }
}

extern inline void func_800A4018(Vec3 *pos, u8 kind, void *data) {
    FxPoint *o = func_800A18D0(0x36, 0x24);

    if (o != 0) {
        o->data = data;
        o->time = D_8021945C;
        o->pos = *pos;
        o->kind = kind;
    }
}

void func_800A4098(FxTimed *o, int *done);

static inline float lerp(float a, float b, float f) {
    return a + f * (b - a);
}

void func_800A40CC(SprKey *k, float t, float scale, Vec3 *pos, int unused, u8 flag);

extern inline void func_800A42C8(SprRing *k, float t, float scale, Vec3 *pos, int unused, u8 flag) {
    float s;
    u16 ang;
    Gfx cmds[2];
    Gfx *g;

    if (k->t3 < t || t < k->t0) return;
    ang = t * k->spin;
    if (t < k->t1) {
        s = (t - k->t0) / (k->t1 - k->t0);
        s = scale * s;
    } else if (t < k->t2) {
        s = scale;
    } else {
        s = 1.0f - (t - k->t2) / (k->t3 - k->t2);
        s = scale * s;
    }
    g = cmds;
    gDPSetPrimColor(g++, 0, 0, k->prim[0], k->prim[1], k->prim[2], 255);
    gDPSetEnvColor(g++, k->env[0], k->env[1], k->env[2], 0);
    func_800AD6A8(D_803A53A0[k->model], pos, s, s, 0, ang, 0, 0, flag, cmds, 2, 0);
}

void func_800A4468(FxSprite *o);

void func_800A477C(FxLight *o, int *done);

void func_800A4924(FxChainObj *o, int *done);

void func_800A49D0(FxLaunchObj *o, int *done);

void func_800A4A7C(FxBill *o);

void func_800A4D84(FxFlat *o);

void func_800A502C(FxFog *o, int *done);

void func_800A5614(FxFlash *o, int *done);

void func_800A5744(FxSnd *o, int *done);

void func_800A5858(FxMesh *o);

void func_800A5BD8(Vec3 *pos, u16 ang, u8 kind, float scale, void *def, void *arg);

void func_800A60E0(Vec3 *pos, u16 ang, u8 kind, int cat, void *arg);
