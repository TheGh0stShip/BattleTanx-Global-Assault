typedef unsigned char u8;
typedef unsigned int u32;

typedef struct {
    u32 w0;
    u32 w1;
} Gfx;

typedef struct {
    int unk0;
    Gfx *dl;
    int unk8;
} Mesh;

typedef struct {
    Mesh *meshes;
    u8 count;
} Model;

extern Gfx D_801146E8[1];
extern Gfx D_801146F0[1];

static inline void func_80095E7C(Gfx *dl, Gfx *srcb, Gfx *repb) {
    u32 term = 0xDF000000;
    Gfx *end = dl + 1000;
    Gfx *src, *rep, *g;
    int k;

    do {
        if (dl->w0 == 0xDF000000) return;
        k = 0;
        g = dl;
        rep = repb;
        src = srcb;
        do {
            if (src->w0 == g->w0 && src->w1 == g->w1) {
                *g = *rep;
            }
            rep++;
            k++;
            src++;
        } while (k <= 0);
        dl++;
    } while ((int)dl < (int)end);
}

static inline void patch_model(Model *mp) {
    Gfx *srcb;
    Gfx *repb;
    int i;

    srcb = D_801146E8;
    repb = D_801146F0;
    for (i = 0; i < mp->count; i++) {
        if (mp->meshes[i].dl != 0) {
            func_80095E7C(mp->meshes[i].dl, srcb, repb);
        }
    }
}

inline void func_80095F08(Model **mp) {
    if (mp == 0) return;
    patch_model(*mp);
}
