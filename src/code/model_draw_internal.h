#ifndef MODEL_DRAW_INTERNAL_H
#define MODEL_DRAW_INTERNAL_H

typedef unsigned char u8;
typedef unsigned short u16;

typedef struct { float x, y; } ModelDrawVec2;

typedef struct {
    ModelDrawVec2 pos;
    char pad08[0x20];
} ModelDrawView;

typedef struct {
    void *a;
    void *b;
    void *c;
} ModelDrawPart;

typedef struct {
    ModelDrawPart *parts;
    u8 count;
} ModelDrawLod;

typedef struct {
    ModelDrawLod *lods;
    char pad04[8];
    u8 lodMode;
} ModelDrawModel;

typedef struct {
    char pad00[0x30];
    float x;
    float unknown34;
    float y;
} ModelDrawObjectTransform;

typedef struct {
    char pad00[0x18];
    int x;
    int y;
} ModelDrawFixedTransform;

typedef struct {
    void *world;
    int track;
    unsigned int mode;
    u8 nHuman;
    u8 nPlayers;
    u8 nTanks;
    u8 nSlots;
    u8 teams[5];
    char pad13[3];
    int time;
    ModelDrawView views[4];
} ModelDrawGameState;

extern ModelDrawGameState D_80219498;

void func_8007B1F0(void *a, void *c, void *b, void *arg1, int arg4,
                   ModelDrawObjectTransform *object,
                   ModelDrawFixedTransform *fixed, int view, int arg3,
                   int relocated);

static inline int model_draw_select_lod(ModelDrawVec2 *a, ModelDrawVec2 *b,
                                        int kind) {
    float dx;
    float dy;
    float distance;

    switch (kind) {
    case 2:
        dx = a->x - b->x;
        dy = a->y - b->y;
        if (90000.0f < dx * dx + dy * dy) {
            return 1;
        }
        return 0;
    case 3:
        dx = a->x - b->x;
        dy = a->y - b->y;
        distance = dx * dx + dy * dy;
        if (2890000.0f < distance) {
            return 2;
        }
        if (1000000.0f < distance) {
            return 1;
        }
        return 0;
    }
    return 0;
}

static inline void model_draw_object_for_view(
    ModelDrawModel *model, void *arg1, ModelDrawObjectTransform *object,
    ModelDrawFixedTransform *fixed, int arg3, int arg4, int view) {
    ModelDrawVec2 position;
    ModelDrawLod *lod;
    int i;

    if (model != 0) {
        if (object != 0) {
            position.x = object->x;
            position.y = object->y;
        } else {
            position.x = fixed->x / 65536.0f;
            position.y = fixed->y / 65536.0f;
        }
        lod = &model->lods[model_draw_select_lod(
            &D_80219498.views[view].pos, &position, model->lodMode)];
        for (i = 0; i < lod->count; i++) {
            ModelDrawPart *part = &lod->parts[i];

            func_8007B1F0(part->a, part->c, part->b, arg1, arg4, object,
                          fixed, view, arg3, 0);
        }
    }
}

#endif
