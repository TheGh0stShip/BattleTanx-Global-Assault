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

extern World *D_80219498;
extern ModelOut *D_803A53A0[0x107];
extern u8 D_B03F6EE8[];
extern u8 D_B03F9B5C[];
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
s32 func_800BB2A0(WorldBuildState *state);
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

#define ALIGN4(x) (((u32)(x) + 3) & ~3)

static inline void registryAdd(s32 value, s32 key) {
    s32 index = D_803A57E0;

    D_803A57C0[index].value = value;
    D_803A57C0[index].key = key;
    D_803A57E0 = index + 1;
}

s32 func_800BB53C(u8 *dest, s32 capacity, u8 *hdr, s32 unused, void *arg) {
    WorldBuildState *state = arg;
    WorldBundleSections *b;
    s32 i;
    s32 j;
    s32 k;
    s32 size;
    u8 *models;
    u8 *parts;
    u8 *refs;
    u8 *materials;
    u8 *vertices;
    u8 *textures;
    u16 *tbl;
    s32 count;

    _bzero(state, sizeof(WorldBuildState));
    count = 1;
    state->bundle_count = count;
    state->bundles = (WorldBundleSections *)(state + 1);
    _bzero(state->bundles, sizeof(WorldBundleSections));
    for (i = 0; i < count; i++) {
        s32 *h;

        b = &state->bundles[i];
        func_8009ED00(D_B03F6EE8, dest, ((D_B03F9B5C - D_B03F6EE8) + 1) & ~1);
        func_800A0750(D_B03F9B5C - D_B03F6EE8, dest, *(s32 *)dest, hdr);
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
            b->model_base = state->bundles[i - 1].model_base + b->model_count;
            b->part_base = state->bundles[i - 1].part_base + b->part_count;
            b->pool_ref_base = state->bundles[i - 1].pool_ref_base + b->pool_ref_count;
        }
        hdr += h[7];
    }
    if (func_800BB2A0(state) < 0) {
        return -1;
    }
    size = state->model_count * 0x10 + state->part_count * 8 + state->pool_ref_count * 0xC +
           state->material_pool_size + state->vertex_pool_size + state->texture_pool_size + 0x40;
    if (capacity < size) {
        return -1;
    }
    _bzero(dest, size);
    models = dest;
    parts = (u8 *)ALIGN4(models + state->model_count * 0x10);
    refs = (u8 *)ALIGN4(parts + state->part_count * 8);
    materials = (u8 *)ALIGN8(refs + state->pool_ref_count * 0xC);
    vertices = (u8 *)ALIGN8(materials + state->material_pool_size);
    textures = (u8 *)ALIGN8(vertices + state->vertex_pool_size);
    tbl = (u16 *)(state->bundles->object_defs + ((Placement *)state->bundles->placements)->def + 2);
    for (i = 0; i < 0x107; i++) {
        if (D_803A53A0[i] == 0 && tbl[i] != 0xFFFF) {
            ModelOut *o = (ModelOut *)models + tbl[i];
            WorldModel *m;

            D_803A53A0[i] = o;
            m = (WorldModel *)state->bundles->models + tbl[i];
            o->min_x = m->x0;
            o->max_x = m->x1;
            o->min_z = m->z0;
            o->max_z = m->z1;
            o->part_count = m->part_count;
            o->parts = parts + m->first_part * 8;
            for (j = m->first_part; j < m->first_part + o->part_count; j++) {
                PartOut *po = (PartOut *)parts + j;
                WorldPart *p = (WorldPart *)state->bundles->parts + j;

                po->reference_count = p->reference_count;
                po->refs = refs + p->first_reference * 0xC;
                for (k = p->first_reference; k < p->first_reference + po->reference_count; k++) {
                    RefOut *ro = (RefOut *)refs + k;
                    PoolRef *r = (PoolRef *)state->bundles->pool_refs + k;

                    ro->vertex = vertices + vertexOffset(state, r->vertex);
                    if (r->material == -1) {
                        ro->material = 0;
                    } else {
                        ro->material = materials + materialOffset(state, r->material);
                    }
                    ro->texture = textures + textureOffset(state, r->texture);
                }
            }
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
    D_803A57E0 = 0;
    if (D_803A53A0[21] != 0 && D_803A53A0[21] != (ModelOut *)-1) {
        WorldModel *m = (WorldModel *)state->bundles->models + tbl[21];
        WorldPart *p = (WorldPart *)state->bundles->parts + m->first_part;
        PoolRef *r = (PoolRef *)state->bundles->pool_refs + p->first_reference;

        registryAdd((s32)(materials + materialOffset(state, r->material)), r->material);
    }
    return size;
}
