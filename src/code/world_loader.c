typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;

typedef struct {
    u32 w0;
    u32 w1;
} DisplayCommand;

typedef struct WorldBundleSections {
    u8 *groups_header;      /* 0x00 */
    u8 *groups;             /* 0x04 */
    u8 *placements;         /* 0x08 */
    s32 placement_count;    /* 0x0C */
    u8 *object_defs;        /* 0x10 */
    u8 *models;             /* 0x14 */
    s32 model_count;        /* 0x18 */
    s32 model_base;         /* 0x1C */
    u8 *parts;              /* 0x20 */
    s32 part_count;         /* 0x24 */
    s32 part_base;          /* 0x28 */
    u8 *pool_refs;          /* 0x2C */
    s32 pool_ref_count;     /* 0x30 */
    s32 pool_ref_base;      /* 0x34 */
} WorldBundleSections;

typedef struct SizeEntry {
    s32 id;
    s32 size;
    s32 offset;
} SizeEntry;

typedef struct WorldBuildState {
    s32 bundle_count;               /* 0x0000 */
    WorldBundleSections *bundles;   /* 0x0004 */
    s32 texture_count;              /* 0x0008 */
    SizeEntry textures[0x100];      /* 0x000C */
    s32 texture_pool_size;          /* 0x0C0C */
    s32 material_count;             /* 0x0C10 */
    SizeEntry materials[0x100];     /* 0x0C14 */
    s32 material_pool_size;         /* 0x1814 */
    s32 vertex_count;               /* 0x1818 */
    SizeEntry vertices[0xAEE];      /* 0x181C */
    u8 pad_9B44[0x18];
    s32 vertex_pool_size;           /* 0x9B5C */
    s32 max_groups;                 /* 0x9B60 */
    s32 group_object_bytes[12];     /* 0x9B64 */
    s32 object_bytes;               /* 0x9B94 */
    s32 model_count;                /* 0x9B98 */
    s32 part_count;                 /* 0x9B9C */
    s32 pool_ref_count;             /* 0x9BA0 */
    void *world;                    /* 0x9BA4 */
    void *models_out;               /* 0x9BA8 */
} WorldBuildState;

typedef struct {
    s16 x;
    s16 z;
} Corner;

typedef struct {
    Corner min;
    Corner max;
} Box;

typedef struct {
    Box box;                /* 0x00 */
    Box cbox;               /* 0x08 */
    s32 object_count;       /* 0x10 */
    u8 *objects;            /* 0x14 */
    s32 unk18;
    s32 height;             /* 0x1C */
} WorldGroup;

typedef struct {
    s32 unk0;
    WorldGroup groups[6];   /* 0x04 */
    u8 pad[0xC4 - 0xC4];
    u8 group_count;         /* 0xC4 */
} World;

typedef struct {
    u16 count;
    u16 first;
    s16 x0;
    s16 y0;
    s16 z0;
    s16 x1;
    s16 y1;
    s16 z1;
} BundleGroup;

typedef struct {
    s16 x;
    s16 y;
    s16 z;
    u16 ang;
    s32 def;
} Placement;

typedef struct {
    u8 part_count;
    u8 pad1;
    u16 first_part;
    s16 x0;
    s16 y0;
    s16 z0;
    s16 x1;
    s16 y1;
    s16 z1;
} WorldModel;

typedef struct {
    u8 reference_count;
    u8 pad1;
    u16 first_reference;
} WorldPart;

typedef struct {
    s32 vertex;
    s32 unk4;
    s32 texture;
    s32 unkC;
    s32 material;
    s32 unk14;
} PoolRef;

typedef struct {
    u8 *parts;              /* 0x00 */
    s16 min_x;              /* 0x04 */
    s16 min_z;              /* 0x06 */
    s16 max_x;              /* 0x08 */
    s16 max_z;              /* 0x0A */
    u8 part_count;          /* 0x0C */
} ModelOut;

typedef struct {
    u8 *refs;               /* 0x00 */
    u8 reference_count;     /* 0x04 */
} PartOut;

typedef struct {
    u8 *material;           /* 0x00 */
    u8 *texture;            /* 0x04 */
    u8 *vertex;             /* 0x08 */
} RefOut;

typedef struct {
    s32 value;
    s32 key;
} DisplayRegistryEntry;

typedef struct {
    World *world;       /* 0x80219498 */
} GameState;

extern GameState D_80219498;
extern DisplayRegistryEntry D_803A57C0[];
extern s32 D_803A57E0;
extern u8 D_B0102C70[];
extern u8 D_B02F8070[];
extern u8 D_B03013F0[];

void _bzero(void *p, s32 n);
void func_8009ED00(void *src, void *dst, s32 size);
void func_800A0750(s32 size, void *src, s32 word, void *dst);
void func_800ACEB4(s32 size);
s32 func_800B0444(void);
void func_800B06E0(void);
void func_800B9B8C(DisplayCommand *dl, s32 n);
s32 func_800BA330(WorldBuildState *state);
void func_800DFA5C(u8 *def, WorldBuildState *state, float *pos, u16 ang, s32 group, s32 bundle);

static inline s32 regLookup(s32 key) {
    s32 i;

    for (i = 0; i < D_803A57E0; i++) {
        if (key == D_803A57C0[i].key) {
            return D_803A57C0[i].value;
        }
    }
    return 0;
}

static inline s32 materialOffset(WorldBuildState *state, s32 key) {
    s32 i;

    for (i = 0; i < state->material_count; i++) {
        if (state->materials[i].id == key) {
            return state->materials[i].offset;
        }
    }
    return 0;
}

static inline s32 vertexOffset(WorldBuildState *state, s32 key) {
    s32 i;

    for (i = 0; i < state->vertex_count; i++) {
        if (state->vertices[i].id == key) {
            return state->vertices[i].offset;
        }
    }
    return 0;
}

static inline s32 textureOffset(WorldBuildState *state, s32 key) {
    s32 i;

    for (i = 0; i < state->texture_count; i++) {
        if (state->textures[i].id == key) {
            return state->textures[i].offset;
        }
    }
    return 0;
}

static inline void relocFD(DisplayCommand *commands, s32 count, s32 offset) {
    s32 index;

    for (index = 0; index < count; index++) {
        if (commands[index].w0 == 0xDF000000) {
            break;
        }
        if (*(u8 *)&commands[index] == 0xFD) {
            commands[index].w1 += offset;
        }
    }
}

static inline void reloc01(DisplayCommand *commands, s32 count, s32 offset) {
    s32 index;

    for (index = 0; index < count; index++) {
        if (commands[index].w0 == 0xDF000000) {
            break;
        }
        if (*(u8 *)&commands[index] == 1) {
            commands[index].w1 += offset;
        }
    }
}

#define BG(b, i) (((BundleGroup *)state->bundles[b].groups)[i])
#define ALIGN8(x) (((u32)(x) + 7) & ~7)

s32 func_800BA6C0(u8 **starts, u8 **ends, s32 count, u8 *dest, s32 capacity, u8 *hdr, s32 hdr_size,
                  void *arg) {
    WorldBuildState *state = arg;
    WorldBundleSections *b;
    WorldBundleSections *bm;
    WorldBundleSections *bp;
    World *w;
    s32 i;
    s32 bi;
    s32 gj;
    s32 j;
    s32 k;
    s32 size;
    u8 *objects;
    u8 *models;
    u8 *parts;
    u8 *refs;
    u8 *materials;
    u8 *vertices;
    u8 *textures;
    float pos[3];

    _bzero(state, sizeof(WorldBuildState));
    state->bundle_count = count;
    state->bundles = (WorldBundleSections *)(state + 1);
    _bzero(state->bundles, count * sizeof(WorldBundleSections));
    for (i = 0; i < count; i++) {
        s32 *h;

        b = &state->bundles[i];
        func_8009ED00(starts[i], dest, ((ends[i] - starts[i]) + 1) & ~1);
        func_800A0750(ends[i] - starts[i], dest, *(s32 *)dest, hdr);
        h = (s32 *)hdr;
        b->groups_header = hdr + h[0];
        b->groups = hdr + h[1];
        b->placements = hdr + h[2];
        b->object_defs = hdr + h[3];
        b->models = hdr + h[4];
        b->parts = hdr + h[5];
        b->pool_refs = hdr + h[6];
        b->placement_count = (u32)(h[3] - h[2]) / 12;
        b->model_count = (u32)(h[5] - h[4]) >> 4;
        b->part_count = (u32)(h[6] - h[5]) >> 2;
        b->pool_ref_count = (u32)(h[7] - h[6]) / 24;
        if (i == 0) {
            b->model_base = 0;
            b->part_base = 0;
            b->pool_ref_base = 0;
        } else {
            b->model_base = state->bundles[i - 1].model_base + state->bundles[i - 1].model_count;
            b->part_base = state->bundles[i - 1].part_base + state->bundles[i - 1].part_count;
            b->pool_ref_base = state->bundles[i - 1].pool_ref_base + state->bundles[i - 1].pool_ref_count;
        }
        hdr += (h[7] + 7) & ~7;
    }
    if (func_800BA330(state) < 0) {
        return -1;
    }
    size = 0x284 + state->object_bytes * 0x50 + state->model_count * 0x10 + state->part_count * 8 + state->pool_ref_count * 0xC + state->texture_pool_size + state->vertex_pool_size + state->material_pool_size + 0x80;
    if (capacity < size) {
        return -1;
    }
    func_800ACEB4(size);
    _bzero(dest, size);
    w = (World *)dest;
    D_80219498.world = w;
    objects = (u8 *)ALIGN8(dest + 0x284);
    models = (u8 *)ALIGN8(objects + state->object_bytes * 0x50);
    parts = (u8 *)ALIGN8(models + state->model_count * 0x10);
    refs = (u8 *)ALIGN8(parts + state->part_count * 8);
    materials = (u8 *)ALIGN8(refs + state->pool_ref_count * 0xC);
    vertices = (u8 *)ALIGN8(materials + state->material_pool_size);
    textures = (u8 *)ALIGN8(vertices + state->vertex_pool_size);
    w->unk0 = 0;
    w->group_count = state->max_groups;
    for (i = 0; i < w->group_count; i++) {
        w->groups[i].box.min.x = BG(0, i).x0;
        w->groups[i].box.max.x = BG(0, i).x1;
        w->groups[i].box.min.z = BG(0, i).z0;
        w->groups[i].box.max.z = BG(0, i).z1;
        w->groups[i].height = BG(0, i).y1 - BG(0, i).y0;
        for (gj = 1; gj < state->bundle_count; gj++) {
            if (i < *(s32 *)state->bundles[gj].groups_header) {
                if (BG(gj, i).y1 - BG(gj, i).y0 > w->groups[i].height) {
                    w->groups[i].height = BG(gj, i).y1 - BG(gj, i).y0;
                }
                if (BG(gj, i).x0 < w->groups[i].box.min.x) {
                    w->groups[i].box.min.x = BG(gj, i).x0;
                }
                if (BG(gj, i).z0 < w->groups[i].box.min.z) {
                    w->groups[i].box.min.z = BG(gj, i).z0;
                }
                if (w->groups[i].box.max.x < BG(gj, i).x1) {
                    w->groups[i].box.max.x = BG(gj, i).x1;
                }
                if (w->groups[i].box.max.z < BG(gj, i).z1) {
                    w->groups[i].box.max.z = BG(gj, i).z1;
                }
            }
        }
        if (w->groups[i].box.max.x == w->groups[i].box.min.x) {
            w->groups[i].box.min.x = -9000;
            w->groups[i].box.min.z = -9000;
            w->groups[i].box.max.x = 9000;
            w->groups[i].box.max.z = 9000;
        }
        w->groups[i].cbox = w->groups[i].box;
        w->groups[i].object_count = state->group_object_bytes[i];
        if (i == 0) {
            w->groups[0].objects = objects;
        } else {
            w->groups[i].objects = w->groups[i - 1].objects + w->groups[i - 1].object_count * 0x50;
        }
    }
    for (bi = 0; bi < state->bundle_count; bi++) {
        bm = &state->bundles[bi];
        for (i = 0; i < bm->model_count; i++) {
            ModelOut *o = (ModelOut *)models + (i + bm->model_base);
            WorldModel *m = (WorldModel *)bm->models + i;

            o->min_x = m->x0;
            o->max_x = m->x1;
            o->min_z = m->z0;
            o->max_z = m->z1;
            o->part_count = m->part_count;
            o->parts = parts + (m->first_part + bm->part_base) * 8;
        }
        for (i = 0; i < bm->part_count; i++) {
            PartOut *o = (PartOut *)parts + (i + bm->part_base);
            WorldPart *p = (WorldPart *)bm->parts + i;

            o->reference_count = p->reference_count;
            o->refs = refs + (p->first_reference + bm->pool_ref_base) * 0xC;
        }
        for (i = 0; i < bm->pool_ref_count; i++) {
            RefOut *o = (RefOut *)refs + (i + bm->pool_ref_base);
            PoolRef *r = (PoolRef *)bm->pool_refs + i;

            o->vertex = vertices + vertexOffset(state, r->vertex);
            if (r->material == -1) {
                o->material = 0;
            } else {
                o->material = (u8 *)regLookup(r->material);
                if (o->material == 0) {
                    o->material = materials + materialOffset(state, r->material);
                }
            }
            o->texture = textures + textureOffset(state, r->texture);
        }
    }
    for (i = 0; i < state->material_count; i++) {
        func_8009ED00(D_B0102C70 + state->materials[i].id, materials + state->materials[i].offset,
                      state->materials[i].size);
        relocFD((DisplayCommand *)(materials + state->materials[i].offset), state->materials[i].size / 8,
                (s32)(materials + state->materials[i].offset));
        if (func_800B0444() != 0) {
            func_800B9B8C((DisplayCommand *)(materials + state->materials[i].offset), state->materials[i].size / 8);
        }
    }
    for (i = 0; i < state->vertex_count; i++) {
        func_8009ED00(D_B03013F0 + state->vertices[i].id, vertices + state->vertices[i].offset,
                      state->vertices[i].size);
        reloc01((DisplayCommand *)(vertices + state->vertices[i].offset), state->vertices[i].size / 8,
                (s32)(vertices + state->vertices[i].offset));
    }
    for (i = 0; i < state->texture_count; i++) {
        func_8009ED00(D_B02F8070 + state->textures[i].id, textures + state->textures[i].offset,
                      state->textures[i].size);
        if (func_800B0444() != 0) {
            func_800B9B8C((DisplayCommand *)(textures + state->textures[i].offset), state->textures[i].size / 8);
        }
    }
    state->world = w;
    state->models_out = models;
    func_800B06E0();
    for (bi = 0; bi < state->bundle_count; bi++) {
        u8 n;

        bp = &state->bundles[bi];
        k = *(s32 *)bp->groups_header;
        for (n = 0; n < k; n++) {
            BundleGroup *g = (BundleGroup *)bp->groups + n;

            for (j = g->first; j < g->first + g->count; j++) {
                Placement *pl = (Placement *)bp->placements + j;

                pos[0] = pl->x;
                pos[2] = pl->y;
                pos[1] = pl->z;
                func_800DFA5C(bp->object_defs + pl->def, state, pos, pl->ang, n, bi);
            }
        }
    }
    return size;
}
