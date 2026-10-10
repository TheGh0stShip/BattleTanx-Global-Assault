typedef unsigned char u8;

typedef struct { float x, y; } Vec2;

typedef struct {
    Vec2 pos;
    char pad08[0x20];
} View;

typedef struct {
    void *a;
    void *b;
    void *c;
} Part;

typedef struct {
    Part *parts;
    u8 count;
} Lod;

typedef struct {
    Lod *lods;
    char pad04[8];
    u8 lodMode;
} Model;

typedef struct {
    char pad00[0x30];
    float x;
    float unknown34;
    float y;
} ObjectTransform;

typedef struct {
    char pad00[0x18];
    int x;
    int y;
} FixedTransform;

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
    View views[4];
} GameState;

extern GameState D_80219498;

void *func_8007B0E4(void *commands, int count, void *part);
void func_8007B1F0(void *a, void *c, void *b, void *arg1, int arg4,
                   ObjectTransform *object, FixedTransform *fixed, int view,
                   int arg3, int relocated);

static inline int select_lod(Vec2 *a, Vec2 *b, int kind) {
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

static inline void draw_object_for_view(Model *model, void *arg1,
                                        ObjectTransform *object,
                                        FixedTransform *fixed, int arg3,
                                        int arg4, int view) {
    Vec2 position;
    Lod *lod;
    int i;

    if (model != 0) {
        if (object != 0) {
            position.x = object->x;
            position.y = object->y;
        } else {
            position.x = fixed->x / 65536.0f;
            position.y = fixed->y / 65536.0f;
        }
        lod = &model->lods[select_lod(&D_80219498.views[view].pos, &position,
                                     model->lodMode)];
        for (i = 0; i < lod->count; i++) {
            Part *part = &lod->parts[i];

            func_8007B1F0(part->a, part->c, part->b, arg1, arg4, object,
                          fixed, view, arg3, 0);
        }
    }
}

void func_800ADCA0(Model *model, void *arg1, ObjectTransform *object, int arg3,
                   int arg4, u8 mask, void *commands, int commandCount,
                   int relocateCount) {
    int playerCount = D_80219498.nPlayers;

    if (model != 0) {
        if (mask == 0) {
            return;
        }
        if (commands == 0) {
            int view;

            for (view = 0; view < playerCount; view++) {
                if ((mask >> view) & 1) {
                    draw_object_for_view(model, arg1, object, 0, arg3, arg4,
                                         view);
                }
            }
        } else {
            Lod *lod;
            Part *part;
            void *relocatedPart;
            int partIndex;
            int view;

            lod = model->lods;
            for (partIndex = 0; partIndex < lod->count; partIndex++) {
                part = &lod->parts[partIndex];
                if (partIndex < relocateCount) {
                    relocatedPart = func_8007B0E4(commands, commandCount,
                                                  part->c);
                } else {
                    relocatedPart = part->c;
                }
                for (view = 0; view < playerCount; view++) {
                    if ((mask >> view) & 1) {
                        func_8007B1F0(part->a, relocatedPart, part->b, arg1,
                                      arg4, object, 0, view, arg3,
                                      partIndex < relocateCount);
                    }
                }
            }
        }
    }
}
